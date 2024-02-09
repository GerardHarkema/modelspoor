// !!! This is an automated generated header file, do not modify by your self !!!
// Timestamp: 09/02/2024 17:29:28
#ifndef _TURNOUT_CONFIG_
#define _TURNOUT_CONFIG_

// Agent config generated from: ../../config/micro_ros_agent_config.json
#define SSID   "BirdsBoven"
#define PASSWORD   "Highway12!"
uint8_t ip_address[4] = {192, 168, 2, 150};
#define PORT   8888

// Turnout config generated from: /home/gerard/modelspoor_ws/src/config/turnout_config_b.json
#define  NODE_NAME  "turnout_group_b"

#define  STATUS_LED  23

TURNOUT_CONFIG turnout_config[] = {
		{MAGNET, 21, {.magnet = {33, 32}}},
		{MAGNET, 22, {.magnet = {26, 25}}},
		{MAGNET, 23, {.magnet = {14, 27}}},
		{MAGNET, 24, {.magnet = {13, 12}}},
		};
#define  NUMBER_OF_TURNOUTS  4

#endif //_TURNOUT_CONFIG_
