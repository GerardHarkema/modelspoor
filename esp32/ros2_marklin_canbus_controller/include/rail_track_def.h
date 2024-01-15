#ifndef RAIL_TRACK_DEF
#define RAIL_TRACK_DEF

#include <micro_ros_platformio.h>

#define SSID          "BirdsModelspoor"
#define PASSWORD      "Highway12!"
IPAddress agent_ip(192, 168, 2, 150);

#define AGENT_PORT      8888

unsigned short int active_turnouts_c[] = {1,2,3,4,5,6,7}; // C-rail active turnouts
#define  NUMBER_OF_ACTIVE_TURNOUTS_C    sizeof(active_turnouts_c) / sizeof(unsigned short int)

unsigned short int active_turnouts_m[] = {10, 11, 12, 13, 14, 15, 16}; // M-rail active turnouts
#define  NUMBER_OF_ACTIVE_TURNOUTS_M    sizeof(active_turnouts_m) / sizeof(unsigned short int)

#endif