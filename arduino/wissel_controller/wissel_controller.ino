#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/bool.h>

#if !defined(ESP32) && !defined(TARGET_PORTENTA_H7_M7) && !defined(ARDUINO_NANO_RP2040_CONNECT) && !defined(ARDUINO_WIO_TERMINAL)
#error This example is only avaible for Arduino Portenta, Arduino Nano RP2040 Connect, ESP32 Dev module and Wio Terminal
#endif

rcl_publisher_t publisher[2];
rcl_subscription_t subscriber[2];
rclc_executor_t executor;
std_msgs__msg__Bool status[2] = {0};
std_msgs__msg__Bool control[2] = {0};
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define LED_PIN 13

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

#define WISSEL_ID  10
#define WISSEL_ID_A  WISSEL_ID
#define WISSEL_ID_B  (WISSEL_ID + 1)

#define SSID          "VRV9517724283"
#define PASSWORD      "@AYCwXhz976C"
#define AGENT_IP      "192.168.2.50"

void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_BUILTIN));
    delay(100);
  }
}

void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    RCSOFTCHECK(rcl_publish(&publisher[0], &status[0], NULL));
    RCSOFTCHECK(rcl_publish(&publisher[1], &status[1], NULL));
    status[0].data != status[0].data;
    status[1].data != status[1].data;
  }
}

void wissel_control_callback0(const void * msgin)
{  
  const std_msgs__msg__Bool * control = (const std_msgs__msg__Bool *)msgin;
  digitalWrite(LED_BUILTIN, (control->data == 0) ? LOW : HIGH);  
  status[0] = control[0];
}

void wissel_control_callback1(const void * msgin)
{  
  const std_msgs__msg__Bool * control = (const std_msgs__msg__Bool *)msgin;
  //digitalWrite(LED_BUILTIN, (control->data == 0) ? LOW : HIGH);  
  status[1] = control[1];
}

void setup() {

  //Serial.begin(115200);
  set_microros_wifi_transports(SSID, PASSWORD, AGENT_IP, 8888);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  char node_name[40];
  sprintf(node_name, "wissel_controller_node%i" , WISSEL_ID_A);
  RCCHECK(rclc_node_init_default(&node, node_name, "", &support));

  char topic_name[40];
  sprintf(topic_name, "wissel%i/status" , WISSEL_ID_A);
  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &publisher[0],
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
    topic_name));
  sprintf(topic_name, "wissel%i/status" , WISSEL_ID_B);
  RCCHECK(rclc_publisher_init_best_effort(
    &publisher[1],
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),  topic_name));
  status[0].data = false;
  status[1].data = true;

  // create subscriber
  sprintf(topic_name, "wissel%i/control" , WISSEL_ID_A);
  RCCHECK(rclc_subscription_init_default(
    &subscriber[0],
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
    topic_name));
  // create subscriber
  sprintf(topic_name, "wissel%i/control" , WISSEL_ID_B);
  RCCHECK(rclc_subscription_init_default(
    &subscriber[1],
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
    topic_name));
  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber[0], &control[0], &wissel_control_callback0, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber[1], &control[1], &wissel_control_callback1, ON_NEW_DATA));

  

}

void loop() {
    RCSOFTCHECK(rcl_publish(&publisher[0], &status[0], NULL));
    RCSOFTCHECK(rcl_publish(&publisher[1], &status[1], NULL));
    RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
    //Serial.printf(".");
    //status.data != status.data;
}
