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
  
  Serial.begin(115200);

  /* Enable ADC1173, set /OE to LOW */
  A.regWrite(GPE_OE, 0x80);
  v = A.regRead(GPE_IDATA);
  v &= ~0x80;
  A.regWrite(GPE_ODATA, v);
}

/* read ADC_data and return Voltage */
unsigned long /* Voltage(mv) */readAdcData(void){
  int adcData;
  unsigned long voltage;
  
  // read ADC value
  adcData = A.regRead(ADC_DATA); 

  /*
   * ADC_data Transform to  Voltage(mv)
   * if yu want to know detail,
   * you can come [http://www.ti.com/product/ADC1173]
   */
  voltage = (unsigned long)adcData * 3300 / 256; 
  
  return voltage;
}

/* write Voltage(mv) to DAC */
void writeDacData(unsigned int voltVal/* Voltage(mv) */){
  unsigned long dacData;
  
  /*
   * Voltage(mv) Transform to ADC_data
   * if yu want to know detail,
   * you can come [http://www.ti.com/product/DAC7311]
   */
  dacData = (unsigned long)voltVal * 4096 / 3300;
  
  // DATA1 first
  A.regWrite(DAC_DATA1, (dacData >> 6) & 0x3F);
  // DATA0 last
  A.regWrite(DAC_DATA0, (dacData << 2) & 0xFC);
}

// the loop routine runs over and over again forever:
void loop() {
  unsigned long voltVal;
  int adcData;

  // read ADC value
  voltVal = readAdcData();
  
  /* output Voltage(ms) by Serial */
  Serial.print("ADC : ");
  Serial.print(voltVal);
  Serial.print(" mV ");

  // output DAC val
  // DAC-OUT = ADC-IN ,input DAC value ,which is what you read before 
  writeDacData(voltVal);
  
  // Change other line
  Serial.println();  
  // delay in between reads for stability
  delay(1500);      
}
