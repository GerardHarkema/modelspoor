// !!! This is an automated generated header file, do not modify by your self !!!
// Timestamp: 10/02/2024 07:48:15
#ifndef _TURNOUT_CONFIG_
#define _TURNOUT_CONFIG_

// Agent config generated from: ../../config/micro_ros_agent_config.json
#define SSID   "BirdsBoven"
#define PASSWORD   "Highway12!"
uint8_t ip_address[4] = {192, 168, 2, 150};
#define PORT   8888

// Turnout config generated from: /home/gerard/modelspoor_ws/src/config/turnout_multi_config_test.json
#define  NODE_NAME  "turnout_multi_config_test"

#define  STATUS_LED  23

TURNOUT_CONFIG turnout_config[] = {
		{MAGNET, 21, {.magnet = {33, 32}}},
		{SERVO, 22, {.servo = {26, 0, 255}}},
		{ANALOG_OUT, 22, {.analog_out = {26, 0, 255}}},
		{DIGITAL_OUT, 24, {.digital_out = {13, true}}},
		};
#define  NUMBER_OF_TURNOUTS  4

#endif //_TURNOUT_CONFIG_
