#ifndef _ARDUINO_MY_GFX_LIBRARIES_H_
#define _ARDUINO_MY_GFX_LIBRARIES_H_

#include "Arduino_DataBus.h"
#include "databus/Arduino_AVRPAR8.h"
#include "databus/Arduino_UNOPAR8.h"
#include "databus/Arduino_AVRPAR16.h"
#include "databus/Arduino_DUEPAR16.h"
#include "databus/Arduino_ESP32LCD8.h"
#include "databus/Arduino_ESP32LCD16.h"
#include "databus/Arduino_ESP32PAR8.h"
#include "databus/Arduino_ESP32PAR8Q.h"
#include "databus/Arduino_ESP32PAR8QQ.h"
#include "databus/Arduino_ESP32PAR8QQQ.h"
#include "databus/Arduino_ESP32PAR16.h"
#include "databus/Arduino_ESP32PAR16Q.h"
#include "databus/Arduino_ESP32PAR16QQ.h"
#include "databus/Arduino_ESP32QSPI.h"
#include "databus/Arduino_ESP32RGBPanel.h"
#include "databus/Arduino_ESP32S2PAR8.h"
#include "databus/Arduino_ESP32S2PAR8Q.h"
#include "databus/Arduino_ESP32S2PAR16.h"
#include "databus/Arduino_ESP32S2PAR16Q.h"
#include "databus/Arduino_ESP32SPI.h"
#include "databus/Arduino_ESP32SPIDMA.h"
#include "databus/Arduino_ESP8266SPI.h"
#include "databus/Arduino_HWSPI.h"
#include "databus/Arduino_mbedSPI.h"
#include "databus/Arduino_NRFXSPI.h"
#include "databus/Arduino_RPiPicoPAR8.h"
#include "databus/Arduino_RPiPicoPAR16.h"
#include "databus/Arduino_RPiPicoSPI.h"
#include "databus/Arduino_RTLPAR8.h"
#include "databus/Arduino_STM32PAR8.h"
#include "databus/Arduino_SWPAR8.h"
#include "databus/Arduino_SWPAR16.h"
#include "databus/Arduino_SWSPI.h"
#include "databus/Arduino_Wire.h"
#include "databus/Arduino_XL9535SWSPI.h"
#include "databus/Arduino_XCA9554SWSPI.h"

#include "Arduino_GFX.h" // Core graphics library
#if !defined(LITTLE_FOOT_PRINT)
#include "canvas/Arduino_Canvas.h"
#include "canvas/Arduino_Canvas_Indexed.h"
#include "canvas/Arduino_Canvas_3bit.h"
#include "canvas/Arduino_Canvas_Mono.h"
#include "display/Arduino_ILI9488_3bit.h"
#endif // !defined(LITTLE_FOOT_PRINT)

#include "display/Arduino_AXS15231B.h"
#include "display/Arduino_GC9A01.h"
#include "display/Arduino_GC9106.h"
#include "display/Arduino_GC9107.h"
#include "display/Arduino_HX8347C.h"
#include "display/Arduino_HX8347D.h"
#include "display/Arduino_HX8352C.h"
#include "display/Arduino_HX8357A.h"
#include "display/Arduino_HX8357B.h"
#include "display/Arduino_HX8369A.h"
#include "display/Arduino_ILI9225.h"
#include "display/Arduino_ILI9331.h"
#include "display/Arduino_ILI9341.h"
#include "display/Arduino_ILI9342.h"
#include "display/Arduino_ILI9481_18bit.h"
#include "display/Arduino_ILI9486.h"
#include "display/Arduino_ILI9486_18bit.h"
#include "display/Arduino_ILI9488.h"
#include "display/Arduino_ILI9488_18bit.h"
#include "display/Arduino_ILI9806.h"
#include "display/Arduino_JBT6K71.h"
#include "display/Arduino_JD9613.h"
#include "display/Arduino_NT35310.h"
#include "display/Arduino_NT35510.h"
#include "display/Arduino_NT39125.h"
#include "display/Arduino_NV3023.h"
#include "display/Arduino_NV3041A.h"
#include "display/Arduino_OTM8009A.h"
#include "display/Arduino_R61529.h"
#include "display/Arduino_RM67162.h"
#include "display/Arduino_RGB_Display.h"
#include "display/Arduino_SEPS525.h"
#include "display/Arduino_SH1106.h"
#include "display/Arduino_SSD1283A.h"
#include "display/Arduino_SSD1306.h"
#include "display/Arduino_SSD1331.h"
#include "display/Arduino_SSD1351.h"
#include "display/Arduino_ST7735.h"
#include "display/Arduino_ST7789.h"
#include "display/Arduino_ST7796.h"
#include "display/Arduino_WEA2012.h"


#define DF_GFX_SCK 19
#define DF_GFX_MOSI 23
#define DF_GFX_MISO 25 //GFX_NOT_DEFINED
#define DF_GFX_CS 22
#define DF_GFX_DC 21
#define DF_GFX_RST 18
#define DF_GFX_BL 5

Arduino_DataBus *my_create_default_Arduino_DataBus();
Arduino_GFX *my_create_default_Arduino_GFX();

#endif // _ARDUINO_my_GFX_LIBRARIES_H_
