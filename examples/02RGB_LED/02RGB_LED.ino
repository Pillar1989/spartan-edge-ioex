/*
  RGBLEDsetting

  set the RGBLED's R,G,B value,which will change the led color
  
  The MIT License (MIT)
  Copyright (C) 2019  Seeed Technology Co.,Ltd.
*/

// include the SPI library:
#include <spartan-edge-ioex.h>

// initialize the spartan_edge_ioex library
spartan_edge_ioex A;

// the setup routine runs once when you press reset:
void setup() {
  //noting to do
}

// the loop routine runs over and over again forever:
void loop() {
  // set RGBled1/2 green
  A.setRGBLedVal(0, 0, 50, 0);
  A.setRGBLedVal(1, 0, 50, 0);
  delay(1000);

  // set RGBled1/2 red
  A.setRGBLedVal(0, 50, 0, 0);
  A.setRGBLedVal(1, 50, 0, 0);
  delay(1000);

  // set RGBled1/2 blue
  A.setRGBLedVal(0, 0, 0, 50);
  A.setRGBLedVal(1, 0, 0, 50);
  delay(1000);
}
