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

#if !defined(ESP32) && !defined(TARGET_PORTENTA_H7_M7) && !defined(ARDUINO_NANO_RP2040_CONNECT) && !defined(ARDUINO_WIO_TERMINAL)
#error This application is only avaible for Arduino Portenta, Arduino Nano RP2040 Connect, ESP32 Dev module and Wio Terminal
#endif

#include "TrackController.h"

const bool DEBUG = true;

bool TrackPower = false;

TrackController *ctrl;

TrackMessage message;

rcl_publisher_t turnout_state_publisher;
rcl_subscription_t turnout_control_subscriber;
rclc_executor_t executor;

railway_interfaces__msg__TurnoutControl control;

unsigned short int active_turnouts[] = {1,2,3,4,5,6,7};
#define  number_of_active_turnouts    sizeof(active_turnouts) / sizeof(unsigned short int)
bool status[number_of_active_turnouts] = {false};


rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;





#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}


#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

#define WISSEL_ID   10
#define WISSEL_ID_A  WISSEL_ID
#define WISSEL_ID_B  (WISSEL_ID + 1)

#define SSID          "BirdsModelspoor"
#define PASSWORD      "Highway12!"
//#define AGENT_IP      "192.168.2.27"
IPAddress agent_ip(192, 168, 2, 27);

#define AGENT_PORT 8888

void error_loop(){
  while(1){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(100);
  }
}


void wissel_control_callback(const void * msgin)
{  
  const railway_interfaces__msg__TurnoutControl * control = (const railway_interfaces__msg__TurnoutControl *)msgin;
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
  sprintf(node_name, "turnout_decoder_node_%i" , WISSEL_ID_A);
  RCCHECK(rclc_node_init_default(&node, node_name, "", &support));

  char topic_name[40];
  sprintf(topic_name, "turnout/status");
  // create turnout_state_publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &turnout_state_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, TurnoutState),
    topic_name));


  // create turnout_control_subscriber
  sprintf(topic_name, "turnout/control" , WISSEL_ID_A);
  RCCHECK(rclc_subscription_init_default(
    &turnout_control_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, TurnoutControl),
    topic_name));

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
        break;
      default:
        break;
    }

  }
  delay(20);


  railway_interfaces__msg__TurnoutControl msg;

#if 0
  msg.number = WISSEL_ID_A;
  msg.state = status[0];
  RCSOFTCHECK(rcl_publish(&turnout_state_publisher, &msg, NULL));
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));

  msg.number = WISSEL_ID_B;
  msg.state = status[1];
  RCSOFTCHECK(rcl_publish(&turnout_state_publisher, &msg, NULL));
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
#endif
}