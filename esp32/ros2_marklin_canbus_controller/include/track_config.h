// !!! This is an automated generated header file, do not modify by your self !!!
// Timestamp: 31/01/2024 17:18:26
#ifndef _TRACK_CONFIG_
#define _TRACK_CONFIG_

// Agent config generated from: ../../config/micro_ros_agent_config.json
#define SSID   "BirdsBoven"
#define PASSWORD   "Highway12!"
uint8_t ip_address[4] = {192, 168, 2, 150};
#define PORT   8888

// Track config generated from: ../../config/track_config.json
LOCOMOTIVE active_locomotives[] = {{5, MFX, 0},{3, DCC, 0},{6, MFX,  0}};
#define  NUMBER_OF_ACTIVE_LOCOMOTIVES   3

unsigned short int active_turnouts_c[] = {1, 2, 3, 4};
#define  NUMBER_OF_ACTIVE_TURNOUTS_C   4

unsigned short int active_turnouts_m[] = {10, 11, 12, 13, 14, 15, 16};
#define  NUMBER_OF_ACTIVE_TURNOUTS_M   7

#endif //_TRACK_CONFIG_
