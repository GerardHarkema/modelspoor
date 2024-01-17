/*********************************************************************
 * Railuino - Hacking your Märklin
 *
 * Copyright (C) 2012 Joerg Pleumann
 * 
 * This example is free software; you can redistribute it and/or
 * modify it under the terms of the Creative Commons Zero License,
 * version 1.0, as published by the Creative Commons Organisation.
 * This effectively puts the file into the public domain.
 *
 * This example is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * LICENSE file for more details.
 */
 
#include "TrackController.h"

const bool DEBUG = true;

TrackController *ctrl;

TrackMessage message;

void setup() {
  Serial.begin(115200);
  while (!Serial);
#if 0
  Serial.print("MOSI: ");Serial.println(MOSI);
  Serial.print("MISO: ");Serial.println(MISO);
  Serial.print("SCK: ");Serial.println(SCK);
  Serial.print("SS: ");Serial.println(SS);  
#endif
  ctrl = new TrackController(0xdf24, DEBUG);
  
  if(DEBUG){
    Serial.println();
    Serial.println();
    Serial.println("DIR CMND R HASH HASH LNGT DAT0 DAT1 DAT2 DAT3 DAT4 DAT5 DAT6 DAT7");
    Serial.println("--- ---- - ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----");
  }
  ctrl->begin();


}
bool once = true;

void loop() {
  if(ctrl->receiveMessage(message)){
#if 1
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
  }
  delay(20);
}
