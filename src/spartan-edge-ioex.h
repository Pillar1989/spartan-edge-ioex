/*
 * spartan-edge-ioex.h - LED libraries for spartan-edge-accelerato
 * 
 */
 
// ensure this library description is only included once
#ifndef SPARTAN_EDGE_IOEX_H
#define SPARTAN_EDGE_IOEX_H 

#include <SPI.h>
#include <Wire.h>

enum {
  GPA_OE = 0x00,
  GPA_ODATA,
  GPA_IDATA,

  GPB_OE = 0x04,
  GPB_ODATA,
  GPB_IDATA,

  GPC_OE = 0x08,
  GPC_ODATA,
  GPC_IDATA,
  #define GPC_ALT_UART_TX   0x01
  #define GPC_ALT_UART_RX   0x02
  #define GPC_ALT_UART_MASK (GPC_ALT_UART_TX | GPC_ALT_UART_RX)
  GPC_ALT,

  GPD_OE = 0x0C,
  GPD_ODATA,
  GPD_IDATA,

  GPE_OE = 0x10,
  GPE_ODATA,
  GPE_IDATA,

  SK6805_CTRL = 0x14,
  SK6805_DATA,

  DAC_DATA0 = 0x16,
  DAC_DATA1,

  UART_DATA = 0x18,
  #define UART_STAT_TX_BUSY  0x10
  #define UART_STAT_RX_DV    0x01
  UART_STAT,

  GPZ_OE = 0x1C,
  GPZ_ODATA,
  GPZ_IDATA,

  ADC_DATA = 0x1F,
  
  WRITE_ADDR = 0b10000000,
};

// library interface description
class spartan_edge_ioex {
  public:
    // constructors:
	spartan_edge_ioex();
	unsigned regRead(int address);
	unsigned regWrite(int address, int value);
	
	// GPIO control
	void GPIO_Init(unsigned int gpioN, unsigned int val);
	unsigned int GPIO_ReadInputDataBit(unsigned int gpioN, unsigned int GPIO_Pin);
	unsigned int GPIO_ReadInputData(unsigned int gpioN);	
	unsigned int GPIO_ReadOutputDataBit(unsigned int gpioN, unsigned int GPIO_Pin);
	unsigned int GPIO_ReadOutputData(unsigned int gpioN);
	
	// addr control
	void GPIO_SetBits(unsigned int addr, unsigned int GPIO_Pin);
	void GPIO_ResetBits(unsigned int addr, unsigned int GPIO_Pin);
	void GPIO_WriteBit(unsigned int addr, unsigned int GPIO_Pin, unsigned int BitVal);
	
  private:
	// SPI2GPIO write
	const byte WRITE = WRITE_ADDR;  

	// set pin 10 as the slave select for the digital pot:
	int slaveSelectPin = 10;
	int resetPin = 9;
};

#endif