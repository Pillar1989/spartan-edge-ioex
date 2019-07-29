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
  static unsigned long color = 0xFFUL;
  
  /* set RGB1 value B->R->G */
  A.regWrite(SK6805_CTRL, 0x0);
  A.regWrite(SK6805_DATA, (color >>  3) & 0x1F);// blue
  A.regWrite(SK6805_CTRL, 0x1);
  A.regWrite(SK6805_DATA, (color >> 11) & 0x1F);// red
  A.regWrite(SK6805_CTRL, 0x2);
  A.regWrite(SK6805_DATA, (color >> 19) & 0x1F);// green

  /* set RGB2 value R->G->B */
  A.regWrite(SK6805_CTRL, 0x3);
  A.regWrite(SK6805_DATA, (color >> 19) & 0x1F);// blue
  A.regWrite(SK6805_CTRL, 0x4);
  A.regWrite(SK6805_DATA, (color >>  3) & 0x1F);// red
  A.regWrite(SK6805_CTRL, 0x5);
  A.regWrite(SK6805_DATA, (color >> 11) & 0x1F);// green

  /* set color value */
  switch (color) {
  case 0x0000FFUL: color = 0x00FF00UL; break;
  case 0x00FF00UL: color = 0xFF0000UL; break;
  case 0xFF0000UL: color = 0x0000FFUL; break;
  default:
    color = 0xFFUL; break;
  }
  
  // delay in between reads for stability
  delay(1500);    
}
