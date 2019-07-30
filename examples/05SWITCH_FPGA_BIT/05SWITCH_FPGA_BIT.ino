/*
  05SWITCH_FPGA_BIT

  using i2c to send infomation to ESP32 to switch the fpga-bit.
  you can see information from ESP32 Serial to see what the infomation you send.
  <top-level-directory-SDcard>/overlay/ has some fpga-bit, 
  the boot.py acquiescently load spi2gpio.bit, which depend on the boaed_config.json 
  if you want to load the other one dynamically, you can Refer to this example
  and if you want to load the other one acquiescently,
  you can change the value about overlay_on_boot in boaed_config.json 
  
  The MIT License (MIT)
  Copyright (C) 2019  Seeed Technology Co.,Ltd.
 */

// include the SPI library:
#include <spartan-edge-ioex.h>

// initialize the spartan_edge_ioex library
spartan_edge_ioex ioex;

// the setup routine runs once when you press reset:
void setup() {
  /*
   * Enable TXS0104E-1 for SPI
   * Enable TXS0104E-0 for UART & I2C
   */
  ioex.GPIO_Init(GPIO_PORT_Z, 0xE0);
  /*
   * FPGA_AR_OE2    = High
   * FPGA_AR_OE1    = High
   * FPGA_ESP_IN12  = Low, Enable ESP32 I2C
   */
  ioex.GPIO_Write(GPIO_PORT_Z, 0xC0);

  Wire.begin();
}

// the loop routine runs over and over again forever:
void loop() {
  Wire.beginTransmission(0x20); // Send from machine address and start bit
  Wire.write(0x01);             // Send register address
  Wire.write(0x02);             // send data
  /* 
   * 0x02 ->spi2gpio
   * 0x01 ->hdmi_v1
   * 0x00 ->mipi_camera
   */
  Wire.write(0x5A);             // send data
  Wire.endTransmission();       // send stop bit

  Serial.println("successful");
 
  // delay in between reads for stability
  delay(1500);
}
