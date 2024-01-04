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
#error This example is only avaible for Arduino Portenta, Arduino Nano RP2040 Connect, ESP32 Dev module and Wio Terminal
#endif

rcl_publisher_t publisher;
rcl_subscription_t subscriber;
rclc_executor_t executor;
bool status[2] = {false};
railway_interfaces__msg__TurnoutControl control;

rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define LED_PIN     13
#define RED_PIN_1   27
#define GREEN_PIN_1 25

#define RED_PIN_2   32
#define GREEN_PIN_2 12

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
    digitalWrite(LED_PIN, !digitalRead(LED_BUILTIN));
    delay(100);
  }
}


void wissel_control_callback(const void * msgin)
{  
  const railway_interfaces__msg__TurnoutControl * control = (const railway_interfaces__msg__TurnoutControl *)msgin;
  switch(control->number){
    case WISSEL_ID_A:
      digitalWrite(LED_BUILTIN, (control->state == 0) ? LOW : HIGH);
      if(control->state){
        digitalWrite(RED_PIN_1, HIGH);  
        delay(200);
        digitalWrite(RED_PIN_1, LOW);
      }
      else{
        digitalWrite(GREEN_PIN_1, HIGH);  
        delay(200);
        digitalWrite(GREEN_PIN_1, LOW);
      }   
      status[0] = control->state;
      break;   
    case WISSEL_ID_B:
      if(control->state){
        digitalWrite(RED_PIN_2, HIGH);  
        delay(500);
        digitalWrite(RED_PIN_2, LOW);
      }
      else{
        digitalWrite(GREEN_PIN_2, HIGH);  
        delay(500);
        digitalWrite(GREEN_PIN_2, LOW);
      }
      status[1] = control->state;
      break;
    default:
        Serial.println("Invalid Turnout");

  }
}

void setup() {


  Serial.begin(115200);
  Serial.println("Turnout-decoder started");



//  size_t agent_port = 8888;

  set_microros_wifi_transports(SSID, PASSWORD, agent_ip, (size_t)AGENT_PORT);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(RED_PIN_1, OUTPUT);
  digitalWrite(RED_PIN_1, LOW);

  pinMode(GREEN_PIN_1, OUTPUT);
  digitalWrite(GREEN_PIN_1, LOW);

  pinMode(RED_PIN_2, OUTPUT);
  digitalWrite(RED_PIN_2, LOW);

  pinMode(GREEN_PIN_2, OUTPUT);
  digitalWrite(GREEN_PIN_2, LOW);

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
  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, TurnoutState),
    topic_name));


  // create subscriber
  sprintf(topic_name, "turnout/control" , WISSEL_ID_A);
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(railway_interfaces, msg, TurnoutControl),
    topic_name));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  // gaat dit hierinder goed?
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &control, &wissel_control_callback, ON_NEW_DATA));
}

void loop() {
    railway_interfaces__msg__TurnoutControl msg;

    msg.number = WISSEL_ID_A;
    msg.state = status[0];
    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
    RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));

    msg.number = WISSEL_ID_B;
    msg.state = status[1];
    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
    RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));

}