/*
 * spartan-edge-ioex - ioex library for spi2gpio bitstream
 */
#include <spartan-edge-ioex.h>

/* constructors */
spartan_edge_ioex::spartan_edge_ioex() {
  // set the slaveSelectPin as an output:
  pinMode(slaveSelectPin, OUTPUT);

  // initialize SPI:
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));

  // set the reset Pin as an output
  pinMode(resetPin, OUTPUT);
  
  // reset FPGA logic
  digitalWrite(resetPin, LOW);
  digitalWrite(resetPin, HIGH);  
}

/* read register */
unsigned spartan_edge_ioex::regRead(int address) {
  unsigned v;

  // take the SS pin low to select the chip:
  digitalWrite(this->slaveSelectPin, LOW);
  // send in the address and value via SPI:
  SPI.transfer(address | 0x0);
  v = SPI.transfer(0x0);
  // take the SS pin high to de-select the chip:
  digitalWrite(this->slaveSelectPin, HIGH);

  return v;
}

/* write register */
unsigned spartan_edge_ioex::regWrite(int address, int value) {
  unsigned v;
  
  // take the SS pin low to select the chip:
  digitalWrite(this->slaveSelectPin, LOW);
  // send in the address and value via SPI:
  SPI.transfer(address | this->WRITE);
  v = SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(this->slaveSelectPin, HIGH);

  return v;
}

/* get the GPIO port input/output directions */
unsigned spartan_edge_ioex::getGpioDir(unsigned GPIO_Port) {
  return regRead(GPIO_Port);
}

/* set the GPIO port input/output directions */
void spartan_edge_ioex::setGpioDir(unsigned GPIO_Port, unsigned Dirs) {
  regWrite(GPIO_Port, Dirs);
}

/* read a bit form port input data */
unsigned spartan_edge_ioex::readGpioInputDataBit(unsigned GPIO_Port, unsigned GPIO_Pin) {
  unsigned v = 0;
  
  v = regRead(GPIO_Port + 2);
  return ((v >> GPIO_Pin) & 0x01);
}

/* read port input data */
unsigned spartan_edge_ioex::readGpioInputData(unsigned GPIO_Port) {
  unsigned v = 0;
  
  v = regRead(GPIO_Port + 2);
  return v;
}

/* read a bit form port output data */
unsigned spartan_edge_ioex::readGpioOutputDataBit(unsigned GPIO_Port, unsigned GPIO_Pin) {
  unsigned v = 0;
  
  v = regRead(GPIO_Port + 1);
  return ((v >> GPIO_Pin) & 0x01);
}

/* read port output data */
unsigned spartan_edge_ioex::readGpioOutputData(unsigned GPIO_Port) {
  int v = 0;
  
  v = regRead(GPIO_Port + 1);
  return v;
}

/* GPIO set a bit */
void spartan_edge_ioex::setGpioBits(unsigned GPIO_Port, unsigned GPIO_Pin) {
  unsigned v = 0;
  unsigned val = 1 << GPIO_Pin;
  
  v = regRead(GPIO_Port + 1);
  regWrite(GPIO_Port + 1, v | val);
}

/* GPIO Reset a bit */
void spartan_edge_ioex::resetGpioBits(unsigned GPIO_Port, unsigned GPIO_Pin) {
  unsigned v = 0;
  unsigned val = 1 << GPIO_Pin;
  
  v = regRead(GPIO_Port + 1);
  regWrite(GPIO_Port + 1, v & ~val);
}

/* GPIO write a bit 1 or 0 */
void spartan_edge_ioex::writeGpioBit(unsigned GPIO_Port, unsigned GPIO_Pin, unsigned BitVal) {
  unsigned v = 0;
  unsigned val = 1 << GPIO_Pin;

  v = regRead(GPIO_Port + 1);
  regWrite(GPIO_Port + 1, (v & ~val) | (BitVal << GPIO_Pin));
}

/* GPIO write a port */
void spartan_edge_ioex::writeGpio(unsigned GPIO_Port, unsigned PortVal) {
  regWrite(GPIO_Port + 1, PortVal);
}

/*
 * cantrol RGBled color
 * index should be set 0 or 1 
 * red/green/blue should be in the range of 0 to 255
 */
void spartan_edge_ioex::setRGBLed(
  unsigned index, unsigned char red, unsigned char green, unsigned char blue
) {
  regWrite(SK6805_CTRL, (0x0u + index * 3));// blue
  regWrite(SK6805_DATA, blue);  				
  regWrite(SK6805_CTRL, (0x1u + index * 3));// red
  regWrite(SK6805_DATA, red);   				
  regWrite(SK6805_CTRL, (0x2u + index * 3));// green
  regWrite(SK6805_DATA, green); 
}

/* ADC1173 initialization */
void spartan_edge_ioex::enableAdc(void) {
  int v;
  
  /* Enable ADC1173, set /OE to LOW */
  setGpioDir(GPIO_PORT_E, 0x80);
  v = readGpioInputData(GPIO_PORT_E);
  v &= ~0x80;
  writeGpio(GPIO_PORT_E, v);
}

/* read ADC_data and return Voltage */
unsigned long /* Voltage(mv) */spartan_edge_ioex::readAdcData(void) {
  int adcData;
  unsigned long voltage;
  
  // read ADC value
  adcData = regRead(ADC_DATA); 

  /*
   * ADC_data Transform to  Voltage(mv)
   * if yu want to know detail,
   * you can come [http://www.ti.com/product/ADC1173]
   */
  voltage = (unsigned long)adcData * 3300 / 256; 
  
  return voltage;
}

/* write Voltage(mv) to DAC */
void spartan_edge_ioex::writeDacData(unsigned voltVal/* Voltage(mv) */) {
  unsigned long dacData;
  
  /*
   * Voltage(mv) Transform to ADC_data
   * if yu want to know detail,
   * you can come [http://www.ti.com/product/DAC7311]
   */
  dacData = (unsigned long)voltVal * 4096 / 3300;
  
  // DATA1 first
  regWrite(DAC_DATA1, (dacData >> 6) & 0x3F);
  // DATA0 last
  regWrite(DAC_DATA0, (dacData << 2) & 0xFC);
}

/* this fuc will return the button statu which you input */
unsigned spartan_edge_ioex::readButtonData(unsigned btnNum) {
  return ((readGpioInputData(GPIO_PORT_E) & btnNum) ==0);
}

/* this fuc will return the swith statu which you input */
unsigned spartan_edge_ioex::readSwithData(unsigned switchNum) {
  return ((readGpioInputData(GPIO_PORT_E) & switchNum) !=0);
}

/* set led */
void spartan_edge_ioex::setLed(unsigned ledNum) {
  setGpioBits(GPIO_PORT_B, ledNum);
}

/* clear led */
void spartan_edge_ioex::clearLed(unsigned ledNum) {
  resetGpioBits(GPIO_PORT_B, ledNum);
}

/* led enable */
void spartan_edge_ioex::enableLed(bool en) {
  int v;
	
  v = regRead(GPIO_PORT_B);
  if (en) {
    v |= LED_ENABLE;
  } else {
    v &= ~LED_ENABLE;
  }
  setGpioDir(GPIO_PORT_B, v);
}

/* led toggle */
void spartan_edge_ioex::toggleLed(unsigned ledNum) {
  int v;
	
  v = readGpioOutputData(GPIO_PORT_B);
  writeGpio(GPIO_PORT_B, (v ^ (1 << ledNum)));
}
