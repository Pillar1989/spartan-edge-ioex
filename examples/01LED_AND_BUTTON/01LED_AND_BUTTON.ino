/*
  01LED_AND_BUTTON

  use button USER1 to control LED1/2 reverse

  The MIT License (MIT)
  Copyright (C) 2019  Seeed Technology Co.,Ltd.
 */

// include the SPI library:
#include <spartan-edge-ioex.h>

// initialize the spartan_edge_ioex library
spartan_edge_ioex ioex;

// the setup routine runs once when you press reset:
void setup() {
  /* set LED1/2 as output */
  ioex.ledEnable();
}

// the loop routine runs over and over again forever:
void loop() {
  // Press USER1, led will reverse
  if (1 == ioex.readButtonData(BTN_USER1)) {
    /* LED1/2  blink */
    ioex.ledToggle(LED2);
    ioex.ledSet(LED1);
    delay(250);
    ioex.ledClear(LED1);
    ioex.ledToggle(LED2);
  }
  
  // wait 250ms 
  delay(250);
}
