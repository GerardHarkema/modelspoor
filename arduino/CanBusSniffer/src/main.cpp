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

  ctrl = new TrackController(0xdf24, DEBUG);
  
  Serial.println();
  Serial.println();
  Serial.println("DIR CMND R HASH HASH LNGT DAT0 DAT1 DAT2 DAT3 DAT4 DAT5 DAT6 DAT7");
  Serial.println("--- ---- - ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----");
  ctrl->begin();
}
bool once = true;

void loop() {
  ctrl->receiveMessage(message);

  if (Serial.available()){
    int menuChoice = Serial.parseInt();
    switch (menuChoice) {
      case 1:
        ctrl->setTurnout(0x3000, false);
        break;
      case 2:
        ctrl->setTurnout(0x3000, true);
        break;
      case 3:
        ctrl->setPower(false);
        break;
      case 4:
        ctrl->setPower(true);
        break;
    }
  }
  byte position, power;
  ctrl->getAccessory(0x3000, &position, &power);// Position Rood = 0, Groen = 1
  Serial.println(position);
  //Serial.println(power);


  delay(20);
}
