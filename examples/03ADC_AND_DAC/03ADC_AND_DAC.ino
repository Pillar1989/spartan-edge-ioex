/*
  03DAC_AND_DAC
 
  read ADC information from FPGA and output by Serial
  write DAC value to FPGA

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

  // initialize serial communication at 115200 bits per second: 
  Serial.begin(115200);

  /* Enable ADC1173, set /OE to LOW */
  A.GPIO_Init(GPIO_PORT_E, 0x80);
  v = A.GPIO_ReadInputData(GPIO_PORT_E);
  v &= ~0x80;
  A.GPIO_Write(GPIO_PORT_E, v);
}

// the loop routine runs over and over again forever:
void loop() {
  unsigned long voltVal;
  int adcData;

  // read ADC value
  voltVal = A.readAdcData();
  
  /* output Voltage(ms) by Serial */
  Serial.print("ADC : ");
  Serial.print(voltVal);
  Serial.print(" mV ");

  // output DAC val
  // DAC-OUT = ADC-IN ,input DAC value ,which is what you read before 
  A.writeDacData(voltVal);
  
  // Change other line
  Serial.println();  
  // delay in between reads for stability
  delay(1500);
}
