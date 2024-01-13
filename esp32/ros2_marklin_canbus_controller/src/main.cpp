#include <Arduino.h>

#include <micro_ros_platformio.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/bool.h>

#include <railway_interfaces/msg/turnout_control.h>
#include <railway_interfaces/msg/turnout_state.h>

#include "rail_track_def.h"

#if !defined(ESP32) && !defined(TARGET_PORTENTA_H7_M7) && !defined(ARDUINO_NANO_RP2040_CONNECT) && !defined(ARDUINO_WIO_TERMINAL)
#error This application is only avaible for Arduino Portenta, Arduino Nano RP2040 Connect, ESP32 Dev module and Wio Terminal
#endif

#include "TrackController.h"

const bool DEBUG = true;

bool TrackPower = false;

TrackController *ctrl;

TrackMessage message;

rcl_publisher_t turnout_status_publisher;
rcl_publisher_t turnout_control_publisher;
rcl_subscription_t turnout_control_subscriber;
rclc_executor_t executor;

railway_interfaces__msg__TurnoutControl control;

bool turnout_status[NUMBER_OF_ACTIVE_TURNOUTS_C] = {false};

rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

rcl_timer_t timer;
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}


void error_loop(){
  while(1){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(100);
  }
}

bool lookupTurnoutIndex(int turnout_number, int *turnout_index){
  int i;
  for(i = 0; i < NUMBER_OF_ACTIVE_TURNOUTS_C; i++){
    if(active_turnouts_c[i] = turnout_number) break;
  }
  if(i = NUMBER_OF_ACTIVE_TURNOUTS_C) return false;
  *turnout_index = i;
  return true;
}

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {

    railway_interfaces__msg__TurnoutState msg;

    for(int i = 0; i < NUMBER_OF_ACTIVE_TURNOUTS_C; i++){
      msg.number = active_turnouts_c[i];
      msg.state = turnout_status[i];
      RCSOFTCHECK(rcl_publish(&turnout_status_publisher, &msg, NULL));
    }

  }
}


void wissel_control_callback(const void * msgin)
{  
  const railway_interfaces__msg__TurnoutControl * control = (const railway_interfaces__msg__TurnoutControl *)msgin;
  int index;
  boolean straight = control->state ? true : false;
  // update controller always !!!
  ctrl->setTurnout(TURNOUT_BASE_ADDRESS + control->number + 1, straight);
  if(lookupTurnoutIndex(control->number, &index)){
    turnout_status[index] = straight;
  }
//  else Serial.println("Invalid Turnout");
}

void setup() {
  Serial.begin(115200);
    while (!Serial);
  Serial.println("Marklin canbus controller started");

  ctrl = new TrackController(0xdf24, DEBUG);
  if(DEBUG){  
    Serial.println();
    Serial.println();
    Serial.println("DIR HASH R CMND LNGT DAT0 DAT1 DAT2 DAT3 DAT4 DAT5 DAT6 DAT7");
    Serial.println("--- ---- - ---- ---- ---- ---- ---- ---- ---- ---- ---- ----");
  }
  ctrl->begin();

  for(int i = 0; i < NUMBER_OF_ACTIVE_TURNOUTS_C; i++){
    ctrl->getTurnout(TURNOUT_BASE_ADDRESS + i + 1, &turnout_status[i]);
  }

//  size_t agent_port = 8888;

  set_microros_wifi_transports(SSID, PASSWORD, agent_ip, (size_t)AGENT_PORT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  char node_name[40];
  sprintf(node_name, "railtrack/canbus_controller");
  RCCHECK(rclc_node_init_default(&node, node_name, "", &support));

  char topic_name[40];
  sprintf(topic_name, "railtrack/turnout/turnout_status");
  // create turnout_status_publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &turnout_status_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, TurnoutState),
    topic_name));


  // create turnout_control_subscriber
  sprintf(topic_name, "railtrack/turnout/control");
  // create turnout_status_publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &turnout_control_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, TurnoutControl),
    topic_name));
  RCCHECK(rclc_subscription_init_default(
    &turnout_control_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, TurnoutControl),
    topic_name));

  // create timer,
  const unsigned int timer_timeout = 1000;
  RCCHECK(rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    timer_callback));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  // gaat dit hierinder goed?
  RCCHECK(rclc_executor_add_subscription(&executor, &turnout_control_subscriber, &control, &wissel_control_callback, ON_NEW_DATA));

}

void loop() {

  if(ctrl->receiveMessage(message)){
#if 1
    Serial.print("COMMAND: ");
    Serial.print("0x");Serial.println(message.command, HEX);
    Serial.print("DLC: ");
    Serial.println(message.length, HEX);
    Serial.print("DATA: ");

    for (int i = 0; i < message.length; i++) {
      Serial.print("0x");Serial.print(message.data[i], HEX);Serial.print(" ");
    }
    
    Serial.println();
  #endif
    switch(message.command){
      case SYSTEM_BEFEHL:
        switch(message.data[4]){
          case SYSTEM_STOP:
              TrackPower = false;
            break;
          case SYSTEM_GO:
              TrackPower = true;
            break;
          default:
            break;
        }
      case ZUBEHOR_SCHALTEN:
          word *position;
          int *adress;
          adress[0] = message.data[0];
          // Testen !!!!
          position[0] = message.data[4];
          railway_interfaces__msg__TurnoutControl msg;

          int turnout_number;
          turnout_number = adress[0] - TURNOUT_BASE_ADDRESS + 1; //? 
          msg.number = turnout_number;
          msg.state = position[0] ? true : false;
          RCSOFTCHECK(rcl_publish(&turnout_control_publisher, &msg, NULL));
          int index;
          if(lookupTurnoutIndex(turnout_number, &index)){
            turnout_status[index] = msg.state;
          }     
  
        break;
      default:
        break;    }
  }
  delay(20);

  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}