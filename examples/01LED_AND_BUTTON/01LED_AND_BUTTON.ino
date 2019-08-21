/*
  01LED_AND_BUTTON

  use button USER1 to control LED1/2 reverse

  The MIT License (MIT)
  Copyright (C) 2019  Seeed Technology Co.,Ltd.
 */

// include the ioex library
#include <spartan-edge-ioex.h>

// initialize the spartan_edge_ioex library
spartan_edge_ioex ioex;

// the setup routine runs once when you press reset:
void setup() {
  /* set LED1/2 as output */
  ioex.enableLed(true);
}

// the loop routine runs over and over again forever:
void loop() {
  // Press USER1, led will reverse
  if (ioex.readButtonData(BTN_USER1)) {
    /* LED1/2  blink */
    ioex.toggleLed(LED2);
    ioex.setLed(LED1);
    delay(250);
    ioex.clearLed(LED1);
    ioex.toggleLed(LED2);
  }
  
  // wait 250ms 
  delay(250);
}
