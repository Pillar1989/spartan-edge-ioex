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

  /*set LED1/2 as output */
  A.GPIO_Init(GPB_OE, 0xC0);
}

// the loop routine runs over and over again forever:
void loop() {
  unsigned v;

  // Press USER1, led will reverse
  if (0 == A.GPIO_ReadInputDataBit(GPE_OE, 4)) { 
    /* LED1/2  reverse */
    A.GPIO_WriteBit(GPB_ODATA, 6, HIGH);
    A.GPIO_WriteBit(GPB_ODATA, 7, HIGH);
    delay(250);
    A.GPIO_WriteBit(GPB_ODATA, 6, LOW);
    A.GPIO_WriteBit(GPB_ODATA, 7, LOW);
  }
  
  // wait 250ms 
  delay(250);
}
