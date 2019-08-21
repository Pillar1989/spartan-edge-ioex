/*
  05SWITCH_FPGA_BIT

  Switch fpga logic/bitstream by sending I2C message to ESP32.

  The ESP32 load a specified bit stream after startup,
  if you want to load the other one dynamically, you can refer to this example.

  The MIT License (MIT)
  Copyright (C) 2019  Seeed Technology Co.,Ltd.
 */

#include <Wire.h>
// include the ioex library
#include <spartan-edge-ioex.h>

// initialize the spartan_edge_ioex library
spartan_edge_ioex ioex;

// the setup routine runs once when you press reset:
void setup() {
  /*
   * Enable TXS0104E-1 for SPI
   * Enable TXS0104E-0 for UART & I2C
   */
  ioex.setGpioDir(GPIO_PORT_Z, 0xE0);
  /*
   * FPGA_AR_OE2    = High
   * FPGA_AR_OE1    = High
   * FPGA_ESP_IN12  = Low, Enable ESP32 I2C
   */
  ioex.writeGpio(GPIO_PORT_Z, 0xC0);

  Serial.begin(115200);
  Wire.begin();
}

// the loop routine runs over and over again forever:
void loop() {
  Wire.beginTransmission(0x20); // Send from machine address and start bit
  Wire.write(0x01);             // Send register address

  /*
   * 0x02 ->spi2gpio
   * 0x01 ->hdmi_v1
   * 0x00 ->mipi_camera
   */
  Wire.write(0x02);             // send data

  Wire.write(0x5A);             // send data
  Wire.endTransmission();       // send stop bit

  Serial.println("successful");

  // delay in between reads for stability
  delay(5000);
}
