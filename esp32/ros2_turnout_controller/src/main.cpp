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

//#include "rail_track_def.h"

#if !defined(ESP32) && !defined(TARGET_PORTENTA_H7_M7) && !defined(ARDUINO_NANO_RP2040_CONNECT) && !defined(ARDUINO_WIO_TERMINAL)
#error This application is only avaible for Arduino Portenta, Arduino Nano RP2040 Connect, ESP32 Dev module and Wio Terminal
#endif


rcl_publisher_t turnout_status_publisher;
rcl_subscription_t turnout_control_subscriber;
rclc_executor_t executor;
railway_interfaces__msg__TurnoutControl turnout_control;


rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

typedef struct{
  int turnout_number;
  int red_pin;
  int green_pin;
}TURNOUT_CONFIG;


#if 0
// For futher implementation
typedef struct{
  int red_pin;
  int green_pin;
}TURNOUT_CONFIG_MAGNET;

typedef struct{
  int pin;
  int red_servo_value;
  int green_servo_value;
}TURNOUT_CONFIG_SERVO;

typedef struct{
  int pin;
  int red_analog_value;
  int green_analog_value;
}TURNOUT_CONFIG_ANALOG_OUT;

typedef struct{
  int pin;
  bool negative_logic;
}TURNOUT_CONFIG_DIGITAL_OUT;

typedef enum{
  MAGNET,
  SERVO,
  ANALOG_OUT,
  DIGITAL_OUT
}TURNOUT_TYPE;

typedef union turnout
{
  TURNOUT_CONFIG_MAGNET magnet;
  TURNOUT_CONFIG_SERVO servo;
  TURNOUT_CONFIG_ANALOG_OUT analog_out;
  TURNOUT_CONFIG_DIGITAL_OUT digital_out;
}TURNOUT;

typedef struct{
  TURNOUT_TYPE type;
  int turnout_number;
  TURNOUT turnout;
}TURNOUT_CONFIG_EX;

/* Example of using TURNOUT_CONFIG_EX 
TURNOUT_CONFIG_EX test;
test.type = MAGNET;
test.turnout_number = MAGNET;
test.type = MAGNET;
test.turnout.magnet.red_pin = 12;
*/


//hoe werkt dit???
TURNOUT_CONFIG_EX test[] = {{MAGNET, 10 ,10, 11} 
                           ,{SERVO, 12 ,servo.pin = 10, servo.red_vale = 100, servo.green_value =  800}
                            };

// End For futher implementation
#endif

#include "turnout_config.h"

IPAddress agent_ip(ip_address[0], ip_address[1], ip_address[2], ip_address[3]);

railway_interfaces__msg__TurnoutState turnout_status[NUMBER_OF_TURNOUTS] = {0};

rcl_timer_t timer;
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

bool lookupTurnoutIndex(int turnout_number, int *turnout_index){
  int i;
  for(i = 0; i < NUMBER_OF_TURNOUTS; i++){
    if(turnout_config[i].turnout_number == turnout_number) break;
  }
  if(i == NUMBER_OF_TURNOUTS) return false;
  *turnout_index = i;
  return true;
}

void error_loop(){
  while(1){
    digitalWrite(STATUS_LED, !digitalRead(STATUS_LED));
    delay(100);
  }
}


void turnout_control_callback(const void * msgin)
{ 
  int turnout_index;
  const railway_interfaces__msg__TurnoutControl * control = (const railway_interfaces__msg__TurnoutControl *)msgin;
  //Serial.println("callback");
  //Serial.println(control->number);
  if(lookupTurnoutIndex(control->number, &turnout_index)){
      //Serial.println("set turnout");
      uint pin = control->state ? turnout_config[turnout_index].green_pin : turnout_config[turnout_index].red_pin;

      digitalWrite(pin, HIGH);  
      delay(200);
      digitalWrite(pin, LOW);  

      turnout_status[turnout_index].state = control->state ? true : false;
      EEPROM.writeBool(turnout_index, turnout_status[turnout_index].state);
      EEPROM.commit();
  }
  else Serial.println("Invalid Turnout");

}

int turnout_state_index = 0;

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    //Serial.print("x");
    RCSOFTCHECK(rcl_publish(&turnout_status_publisher, &turnout_status[turnout_state_index], NULL));
    turnout_state_index++;
    if(turnout_state_index == NUMBER_OF_TURNOUTS)turnout_state_index = 0;
  }
}


void setup() {

  Serial.begin(115200);
  while (!Serial);
  delay(2000);
  Serial.print("Turnout-decoder started, node: ");
  Serial.println(NODE_NAME);

  EEPROM.begin(NUMBER_OF_TURNOUTS);

  set_microros_wifi_transports(SSID, PASSWORD, agent_ip, (size_t)PORT);

  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, HIGH);

  for(int i=0; i < NUMBER_OF_TURNOUTS; i++){
    pinMode(turnout_config[i].green_pin, OUTPUT);
    digitalWrite(turnout_config[i].green_pin, LOW);

    pinMode(turnout_config[i].red_pin, OUTPUT);
    digitalWrite(turnout_config[i].red_pin, LOW);

    turnout_status[i].number = turnout_config[i].turnout_number;
    turnout_status[i].state = EEPROM.readBool(i);
  }

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, NODE_NAME, "", &support));

  char topic_name[40];
  sprintf(topic_name, "railtrack/turnout/status");
  // create turnout_status_publisher
  RCCHECK(rclc_publisher_init_default(
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
  const unsigned int timer_timeout = 500/NUMBER_OF_TURNOUTS;
  RCCHECK(rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    timer_callback));

  // create executor
  int number_of_executors = 2;
  RCCHECK(rclc_executor_init(&executor, &support.context, number_of_executors, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));
  RCCHECK(rclc_executor_add_subscription(&executor, &turnout_control_subscriber, &turnout_control, &turnout_control_callback, ON_NEW_DATA));

  Serial.println("Turnout-decoder ready");
}

void loop() {

  delay(100);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));

}
