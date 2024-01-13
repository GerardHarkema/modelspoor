#include <Arduino.h>

#include <EEPROM.h>

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
#error This example is only avaible for Arduino Portenta, Arduino Nano RP2040 Connect, ESP32 Dev module and Wio Terminal
#endif

rcl_publisher_t turnout_status_publisher;
rcl_subscription_t turnout_control_subscriber;
rclc_executor_t executor;
railway_interfaces__msg__TurnoutControl control;


rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

typedef struct{
  int turnout_number;
  int red_pin;
  int green_pin;
}TURNOUT_CONFIG;

TURNOUT_CONFIG turnout_config[] ={{10, 27, 25},
                                  {11, 32, 12}};

int number_of_turnouts = sizeof(turnout_config)/sizeof(TURNOUT_CONFIG);  

bool *turnout_status;

rcl_timer_t timer;
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

bool lookupTurnoutIndex(int turnout_number, int *turnout_index){
  int i;
  for(i = 0; i < number_of_turnouts; i++){
    if(turnout_config[i].turnout_number = turnout_number) break;
  }
  if(i = number_of_turnouts) return false;
  *turnout_index = i;
  return true;
}

void error_loop(){
  while(1){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(100);
  }
}


void wissel_control_callback(const void * msgin)
{ 
  int turnout_index;
  const railway_interfaces__msg__TurnoutControl * control = (const railway_interfaces__msg__TurnoutControl *)msgin;
  if(lookupTurnoutIndex(control->number, &turnout_index)){
      uint pin = control->state ? turnout_config[turnout_index].green_pin : turnout_config[turnout_index].red_pin;

      digitalWrite(pin, HIGH);  
      delay(200);
      digitalWrite(pin, LOW);  

      turnout_status[turnout_index] = control->state ? true : false;
      EEPROM.writeBool(turnout_index, turnout_status[turnout_index]);
  }
//  else Serial.println("Invalid Turnout");

}

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    railway_interfaces__msg__TurnoutState msg;

    for(int i = 0; i < number_of_turnouts; i++){
      msg.number = turnout_config[i].turnout_number;
      msg.state = turnout_status[i];
      RCSOFTCHECK(rcl_publish(&turnout_status_publisher, &msg, NULL));
    }
  }
}


void setup() {


  Serial.begin(115200);
  Serial.println("Turnout-decoder started");

  turnout_status = (bool *)malloc(number_of_turnouts * sizeof(bool));

  EEPROM.begin(number_of_turnouts);

//  size_t agent_port = 8888;

  set_microros_wifi_transports(SSID, PASSWORD, agent_ip, (size_t)AGENT_PORT);


  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  for(int i=0; i < number_of_turnouts; i++){
    pinMode(turnout_config[i].green_pin, OUTPUT);
    digitalWrite(turnout_config[i].green_pin, LOW);

    pinMode(turnout_config[i].red_pin, OUTPUT);
    digitalWrite(turnout_config[i].red_pin, LOW);
    turnout_status[i] = EEPROM.readBool(i);
  }

  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  char node_name[40];
  sprintf(node_name, "turnout_decoder_node_%i" , turnout_config[0].turnout_number);
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

  delay(100);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));

}
