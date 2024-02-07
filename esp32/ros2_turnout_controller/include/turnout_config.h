// !!! This is an automated generated header file, do not modify by your self !!!
// Timestamp: 07/02/2024 19:28:04
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

TURNOUT_CONFIG turnout_config[] = {{21, 33, 32},{22, 26, 25},{23, 14, 27},{24, 13, 12}};
#define  NUMBER_OF_TURNOUTS  4

#endif //_TURNOUT_CONFIG_
