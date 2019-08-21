/*
  RGBLEDsetting

  set the RGBLED's R,G,B value,which will change the led color
  
  The MIT License (MIT)
  Copyright (C) 2019  Seeed Technology Co.,Ltd.
 */

// include the ioex library
#include <spartan-edge-ioex.h>

// initialize the spartan_edge_ioex library
spartan_edge_ioex ioex;

// the setup routine runs once when you press reset:
void setup() {
  // noting to do
}

// the loop routine runs over and over again forever:
void loop() {
  // set RGBled1/2 green
  ioex.setRGBLed(RGB_LED1, 0, 50, 0);
  ioex.setRGBLed(RGB_LED0, 0, 50, 0);
  delay(1000);

  // set RGBled1/2 red
  ioex.setRGBLed(RGB_LED1, 50, 0, 0);
  ioex.setRGBLed(RGB_LED0, 50, 0, 0);
  delay(1000);

  // set RGBled1/2 blue
  ioex.setRGBLed(RGB_LED1, 0, 0, 50);
  ioex.setRGBLed(RGB_LED0, 0, 0, 50);
  delay(1000);
}
