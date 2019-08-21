# spartan-edge-ioex
#### a library for Spartan Edge Accelerator Board

This Arduino library allows you control GPIO/ADC/DAC/RGB-LED of Spartan Edge Accelerator Board from Arduino(UNO).

It need the FPGA(xc7s15) have loaded 
[spi2gpio](https://github.com/sea-s7/Demo_project/tree/master/spi2gpio) 
logic, the register/port information also defined there.

Refer [spartan-edge-esp32-boot](https://github.com/sea-s7/spartan-edge-esp32-boot) 
to find out how to load spi2gpio bitstream.
