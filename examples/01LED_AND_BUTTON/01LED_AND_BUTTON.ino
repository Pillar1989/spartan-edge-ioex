/*
  01LED_AND_BUTTON

  use button USER1 to control LED1/2 reverse

  The MIT License (MIT)
  Copyright (C) 2019  Seeed Technology Co.,Ltd.
 */

// include the SPI library:
#include <spartan-edge-ioex.h>

// initialize the spartan_edge_ioex library
spartan_edge_ioex A;

// the setup routine runs once when you press reset:
void setup() {
  int v;

  /* set LED1/2 as output */
  A.ledToggle(LED_ENABLE);
}

// the loop routine runs over and over again forever:
void loop() {
  unsigned v;

  // Press USER1, led will reverse
  if (1 == A.readButtonData(BTN_USER1)) {
    /* LED1/2  blink */
    A.ledSet(LED1);
    A.ledSet(LED2);
    delay(250);
    A.ledClear(LED1);
    A.ledClear(LED2);
  }
  
  // wait 250ms 
  delay(250);
}
