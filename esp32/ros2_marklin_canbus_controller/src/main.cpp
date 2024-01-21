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
#include <railway_interfaces/msg/locomotive_control.h>
#include <railway_interfaces/msg/locomotive_state.h>

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
//rcl_publisher_t turnout_control_publisher;
rcl_subscription_t turnout_control_subscriber;

rcl_publisher_t locomoitive_status_publisher;
rcl_subscription_t locomotive_control_subscriber;

rcl_publisher_t power_status_publisher;
rcl_subscription_t power_control_subscriber;

rclc_executor_t executor;

railway_interfaces__msg__TurnoutControl turnout_control;
railway_interfaces__msg__LocomotiveControl locomotive_control;
std_msgs__msg__Bool power_control;

//bool turnout_status[NUMBER_OF_ACTIVE_TURNOUTS_C] = {false};
railway_interfaces__msg__TurnoutState turnout_status [NUMBER_OF_ACTIVE_TURNOUTS_C] = {0};
railway_interfaces__msg__LocomotiveState locomotive_status[NUMBER_OF_ACTIVE_LOCOMOTIVES] = {0};

rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

rcl_timer_t turnout_state_publisher_timer;
rcl_timer_t locomotive_state_publisher_timer;
rcl_timer_t power_state_publisher_timer;
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void error_loop(){
  Serial.println("Error: System halted");
  while(1){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(100);
  }
}

bool lookupTurnoutIndex(int turnout_number, int *turnout_index){
  int i;
  for(i = 0; i < NUMBER_OF_ACTIVE_TURNOUTS_C; i++){
    if(active_turnouts_c[i] == turnout_number) break;
  }
  if(i == NUMBER_OF_ACTIVE_TURNOUTS_C) return false;
  *turnout_index = i;
  return true;
}

bool lookupLocomotiveIndex(int locomotive_address, int *locomotive_index){
  int i;
  for(i = 0; i < NUMBER_OF_ACTIVE_LOCOMOTIVES; i++){
    if(locomotive_status[i].address == locomotive_address) break;
  }
  if(i == NUMBER_OF_ACTIVE_LOCOMOTIVES) return false;
  *locomotive_index = i;
  return true;
}

int turnout_state_index = 0;

void turnout_state_publisher_timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
#if 1
    //railway_interfaces__msg__TurnoutState turnout_msg;

    //turnout_msg.number = active_turnouts_c[turnout_state_index];
    //turnout_msg.state = turnout_status[turnout_state_index];
    RCSOFTCHECK(rcl_publish(&turnout_status_publisher, &turnout_status[turnout_state_index], NULL));
    turnout_state_index++;
    if(turnout_state_index == NUMBER_OF_ACTIVE_TURNOUTS_C) turnout_state_index = 0;
#endif
  }
}


int locomotive_state_index = 0;

void locomotive_state_publisher_timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    RCSOFTCHECK(rcl_publish(&locomoitive_status_publisher, &locomotive_status[locomotive_state_index], NULL));
    locomotive_state_index++;
    if(locomotive_state_index == NUMBER_OF_ACTIVE_LOCOMOTIVES) turnout_state_index = 0;
  }
}

void power_state_publisher_timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    std_msgs__msg__Bool power_msg;
    power_msg.data = TrackPower;
    RCSOFTCHECK(rcl_publish(&power_status_publisher, &power_msg, NULL));
  }
}

void turnout_control_callback(const void * msgin)
{  
  const railway_interfaces__msg__TurnoutControl * control = (const railway_interfaces__msg__TurnoutControl *)msgin;
  int index;
  boolean straight = control->state ? true : false;
  // update controller always !!!
  if(TrackPower){
    ctrl->setTurnout(TURNOUT_BASE_ADDRESS + control->number - 1, straight);
    if(lookupTurnoutIndex(control->number, &index)){
      turnout_status[index].state = straight;
    }
  }
//  else Serial.println("Invalid Turnout");
}

void locomotive_control_callback(const void * msgin)
{  
  const railway_interfaces__msg__LocomotiveControl * control = (const railway_interfaces__msg__LocomotiveControl *)msgin;

  int locomotive_index;

  switch(control->command){
    case railway_interfaces__msg__LocomotiveControl__SET_SPEED:
      ctrl->setLocoSpeed(control->address, control->speed);
      if(lookupLocomotiveIndex(control->address, &locomotive_index)){
        locomotive_status[locomotive_index].speed = control->speed;
      }
      break;
    case railway_interfaces__msg__LocomotiveControl__SET_DIRECTION:
      ctrl->setLocoDirection(control->address, control->direction);
      if(lookupLocomotiveIndex(control->address, &locomotive_index)){
        locomotive_status[locomotive_index].direction = control->direction;
      }
      break;
    case railway_interfaces__msg__LocomotiveControl__SET_FUNCTION:
      ctrl->setLocoFunction(control->address, control->function_index, control->function_state);
      if(lookupLocomotiveIndex(control->address, &locomotive_index)){
        locomotive_status[locomotive_index].function.data.data[control->function_index] = control->function_state;
      }
      break;
    default:
      Serial.println("Invalid command");
  }
}

void power_control_callback(const void * msgin)
{  
  const std_msgs__msg__Bool * control = (const std_msgs__msg__Bool *)msgin;
  Serial.println("power_control_callback");
  ctrl->setPower(control->data);
  TrackPower = control->data;
//  else Serial.println("Invalid Turnout");
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(2000);
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
    turnout_status[i].number = active_turnouts_c[i];
    turnout_status[i].state = false;
  }

  for(int i = 0; i < NUMBER_OF_ACTIVE_LOCOMOTIVES; i++){
    locomotive_status[i].direction = railway_interfaces__msg__LocomotiveState__DIRECTION_FORWARD;
    switch(active_locomotives->protocol){
      case MM1:
      case MM2:
        locomotive_status[i].address = active_locomotives->id;
        locomotive_status[i].function.data.size = NUMBER_OF_MM1_FUNCTIONS;      
        break;
      case DCC:
        locomotive_status[i].address = active_locomotives->id + ADDR_DCC;
        locomotive_status[i].function.data.size = NUMBER_OF_DCC_FUNCTIONS;      
      break;
      case MFX:
        locomotive_status[i].address = active_locomotives->id + ADDR_MFX;
        locomotive_status[i].function.data.size = NUMBER_OF_MFX_FUNCTIONS;      
      break;

    }
  
      for(int j = 0; j < locomotive_status[i].function.data.size; j++){
        locomotive_status[i].function.data.data[j] = false;
      }
  }


  set_microros_wifi_transports(SSID, PASSWORD, agent_ip, (size_t)AGENT_PORT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  delay(2000);

  allocator = rcl_get_default_allocator();
  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "railtrack_canbus_controller", "", &support));

  char topic_name[40];
  sprintf(topic_name, "railtrack/turnout/status");
  // create turnout_status_publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &turnout_status_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, TurnoutState),
    topic_name));
  
  // create turnout_control_subscriber
  sprintf(topic_name, "railtrack/turnout/control");
  // create turnout_status_publisher
#if 0
  RCCHECK(rclc_publisher_init_best_effort(
    &turnout_control_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, TurnoutControl),
    topic_name));
#endif
  RCCHECK(rclc_subscription_init_default(
    &turnout_control_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, TurnoutControl),
    topic_name));

  sprintf(topic_name, "railtrack/locomotive/status");
  // create locomotive_status_publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &locomoitive_status_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, LocomotiveState),
    topic_name));

  sprintf(topic_name, "railtrack/locomotive/control");
  // create locomotive_control_subscriber
  RCCHECK(rclc_subscription_init_default(
    &locomotive_control_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, LocomotiveControl),
    topic_name));

  sprintf(topic_name, "railtrack/power_status");
  // create power_status_publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &power_status_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
    topic_name));


  sprintf(topic_name, "railtrack/power_control");
  // create power_control_subscriber
  RCCHECK(rclc_subscription_init_default(
    &power_control_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
    topic_name));

  // create timer,
#define CYCLE_TIME    200
  unsigned int timer_timeout = CYCLE_TIME / NUMBER_OF_ACTIVE_TURNOUTS_C;
  RCCHECK(rclc_timer_init_default(
    &turnout_state_publisher_timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    turnout_state_publisher_timer_callback));

  timer_timeout = CYCLE_TIME / NUMBER_OF_ACTIVE_LOCOMOTIVES;
  RCCHECK(rclc_timer_init_default(
    &locomotive_state_publisher_timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    locomotive_state_publisher_timer_callback));

  timer_timeout = CYCLE_TIME;
  RCCHECK(rclc_timer_init_default(
    &power_state_publisher_timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    power_state_publisher_timer_callback));

  // create executor
  int number_of_executors = 6;
  
  RCCHECK(rclc_executor_init(&executor, &support.context, number_of_executors, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &turnout_state_publisher_timer));
  RCCHECK(rclc_executor_add_timer(&executor, &locomotive_state_publisher_timer));
  RCCHECK(rclc_executor_add_timer(&executor, &power_state_publisher_timer));
  RCCHECK(rclc_executor_add_subscription(&executor, &turnout_control_subscriber, &turnout_control, &turnout_control_callback, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_subscription(&executor, &locomotive_control_subscriber, &locomotive_control, &locomotive_control_callback, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_subscription(&executor, &power_control_subscriber, &power_control, &power_control_callback, ON_NEW_DATA));

  Serial.println("!!! Ready for operating !!!");
}

void loop() {

  //Serial.print("*");
#if 1
  if(ctrl->receiveMessage(message)){
#if 0
    Serial.print("COMMAND: ");
    Serial.print("0x");Serial.println(message.command, HEX);
    Serial.print("DLC: ");
    Serial.println(message.length, HEX);
    Serial.print("DATA: ");

    for (int i = 0; i < message.length; i++) {
      Serial.print("0x");Serial.print(message.data[i], HEX);Serial.print(" ");
    }
    Serial.println("");
    int adress;
    adress =  (message.data[0] << 24) 
            + (message.data[1] << 16) 
            + (message.data[2] << 8) 
            + message.data[3];
    Serial.print("Adress (when used): 0x");Serial.println(adress, HEX);
    

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
          word position;
          int adress;
          adress = (message.data[2] << 8) 
                 + message.data[3];
          //Serial.print("Adress: 0x");Serial.println(adress, HEX);
          position = message.data[4];

          int turnout_number;
          turnout_number = adress - TURNOUT_BASE_ADDRESS + 1;
          //Serial.println(turnout_number);

          // Testen op M-Trach turnout!!!!
          for(int i = 0; i < NUMBER_OF_ACTIVE_TURNOUTS_M; i++){
            if(active_turnouts_m[i] == turnout_number){
              railway_interfaces__msg__TurnoutControl msg;
              msg.number = turnout_number;
              msg.state = position ? true : false;
#if 0
              RCSOFTCHECK(rcl_publish(&turnout_control_publisher, &msg, NULL));
#endif
            }
          }
#if 0
          RCSOFTCHECK(rcl_publish(&turnout_control_publisher, &msg, NULL));
          int index;
          if(lookupTurnoutIndex(turnout_number, &index)){
            turnout_status[index] = msg.state;
          }     
#endif
  
        break;
      default:
        break;    }
  }
#endif
  delay(20);

  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}