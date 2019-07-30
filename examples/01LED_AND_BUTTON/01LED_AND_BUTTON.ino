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
  A.GPIO_Init(GPIO_PORT_B, 0xC0);
}

// the loop routine runs over and over again forever:
void loop() {
  unsigned v;

  // Press USER1, led will reverse
  if (0 == A.GPIO_ReadInputDataBit(GPIO_PORT_E, 4)) { 
    /* LED1/2  blink */
    A.GPIO_SetBits(GPIO_PORT_B, 6);
    A.GPIO_SetBits(GPIO_PORT_B, 7);
    delay(250);
    A.GPIO_ResetBits(GPIO_PORT_B, 6);
    A.GPIO_ResetBits(GPIO_PORT_B, 7);
  }
  
  // wait 250ms 
  delay(250);
}
