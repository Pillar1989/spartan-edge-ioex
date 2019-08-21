/*
  04SWITCH

  check the switch and ouput infomation by Serial
  
  The MIT License (MIT)
  Copyright (C) 2019  Seeed Technology Co.,Ltd.
 */

// include the ioex library
#include <spartan-edge-ioex.h>

// initialize the spartan_edge_ioex library
spartan_edge_ioex ioex;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
}

/* check the switch */
int switch_chk(void) {
  unsigned v;
  
  /*
   * read four switchs data
   * if one of the switchs is 'on' ,retun -1.
   * please put it to "off", and try again
   */
  v = ioex.readGpioInputData(GPIO_PORT_E);
  if ((v & (SWITCH_K1 | SWITCH_K2 | SWITCH_K3 | SWITCH_K4)) != 0x00){
      Serial.println("please put switch to off, and try again");
      return -1;
    }
  
  Serial.print("Switch on K1 ");
  // loop forever untill key1 Switched
  for (;;)  if (1 == ioex.readSwithData(SWITCH_K1)) break;
  Serial.println("OK");

  Serial.print("Switch on K2 ");
  // loop forever untill key2 Switched
  for (;;)  if (1 == ioex.readSwithData(SWITCH_K2)) break;
  Serial.println("OK");

  Serial.print("Switch on K3 ");
  // loop forever untill key3 Switched
  for (;;)  if (1 == ioex.readSwithData(SWITCH_K3)) break;
  Serial.println("OK");

  Serial.print("Switch on K4 ");
  // loop forever untill key4 Switched
  for (;;)  if (1 == ioex.readSwithData(SWITCH_K4)) break;
  Serial.println("OK");

  return 0;
}

static int switch_checked = 0;  // avoiding checking looping
// the loop routine runs over and over again forever:
void loop() {
  int r;

  /* switch checking and output info by Serial */
  if (switch_checked == 0) {
    Serial.print("Switch : ");
    Serial.println();
    r = switch_chk();   // checking
    if (r < 0) {
      Serial.print("FAIL ");
      Serial.println(r);
    } else {
      Serial.println("OK");
    }
    // stop checking
    switch_checked = 1; 
  }
  
  // change a line
  Serial.println(); 
  // delay 1.5 s
  delay(1500);      
}
