#include <spartan-edge-ioex.h>

/* constructors */
spartan_edge_ioex::spartan_edge_ioex() {
  int v;
  
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

/* Initialize the GPIO status */
void spartan_edge_ioex::GPIO_Init(unsigned int GPIO_Port, unsigned int PortVal) {
  regWrite(GPIO_Port, PortVal);
}

/* read a bit form port input data */
unsigned int spartan_edge_ioex::GPIO_ReadInputDataBit(unsigned int GPIO_Port, unsigned int GPIO_Pin) {
  unsigned int v = 0;
  
  v = regRead(GPIO_Port + 2);
  return ((v >> GPIO_Pin) & 0x01);
}

/* read port input data */
unsigned int spartan_edge_ioex::GPIO_ReadInputData(unsigned int GPIO_Port) {
  unsigned int v = 0;
  
  v = regRead(GPIO_Port + 2);
  return v;
}

/* read a bit form port output data */
unsigned int spartan_edge_ioex::GPIO_ReadOutputDataBit(unsigned int GPIO_Port, unsigned int GPIO_Pin) {
  unsigned int v = 0;
  
  v = regRead(GPIO_Port + 1);
  return ((v >> GPIO_Pin) & 0x01);
}

/* read port output data */
unsigned int spartan_edge_ioex::GPIO_ReadOutputData(unsigned int GPIO_Port) {
  int v = 0;
  
  v = regRead(GPIO_Port + 1);
  return v;
}

/* GPIO set a bit */
void spartan_edge_ioex::GPIO_SetBits(unsigned int GPIO_Port, unsigned int GPIO_Pin) {
  unsigned int v = 0;
  unsigned int val = 1 << GPIO_Pin;
  
  v = regRead(GPIO_Port + 1);
  regWrite(GPIO_Port + 1, v | val);
}

/* GPIO Reset a bit */
void spartan_edge_ioex::GPIO_ResetBits(unsigned int GPIO_Port, unsigned int GPIO_Pin) {
  unsigned int v = 0;
  unsigned int val = 1 << GPIO_Pin;
  
  v = regRead(GPIO_Port + 1);
  regWrite(GPIO_Port + 1, v & ~val);
}

/* GPIO write a bit 1 or 0 */
void spartan_edge_ioex::GPIO_WriteBit(unsigned int GPIO_Port, unsigned int GPIO_Pin, unsigned int BitVal) {
  unsigned int v = 0;
  unsigned int val = 1 << GPIO_Pin;
  
  v = regRead(GPIO_Port + 1);
  regWrite(GPIO_Port + 1, (v & ~val) | (BitVal << GPIO_Pin));
}

/* GPIO write a port */
void spartan_edge_ioex::GPIO_Write(unsigned int GPIO_Port, unsigned int PortVal) {
  regWrite(GPIO_Port + 1, PortVal);
}

/*
 * cantrol RGBled color
 * index should be set 0 or 1 
 * red/green/blue should be in the range of 0 to 255
 */
void spartan_edge_ioex::setRGBLedVal(unsigned int index, unsigned char red, unsigned char green, unsigned char blue) {
  	regWrite(SK6805_CTRL, (0x0u + index * 3));// blue
	regWrite(SK6805_DATA, blue);  				
	regWrite(SK6805_CTRL, (0x1u + index * 3));// red
	regWrite(SK6805_DATA, red);   				
	regWrite(SK6805_CTRL, (0x2u + index * 3));// green
	regWrite(SK6805_DATA, green); 
}

/* ADC1173 initialization */
void spartan_edge_ioex::adcEnable(void) {
  int v;
  
  /* Enable ADC1173, set /OE to LOW */
  GPIO_Init(GPIO_PORT_E, 0x80);
  v = GPIO_ReadInputData(GPIO_PORT_E);
  v &= ~0x80;
  GPIO_Write(GPIO_PORT_E, v);
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
void spartan_edge_ioex::writeDacData(unsigned int voltVal/* Voltage(mv) */) {
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
unsigned int spartan_edge_ioex::readButtonData(unsigned int btnNum) {
  return ((GPIO_ReadInputData(GPIO_PORT_E) & btnNum) ==0);
}

/* this fuc will return the swith statu which you input */
unsigned int spartan_edge_ioex::readSwithData(unsigned int switchNum) {
  return ((GPIO_ReadInputData(GPIO_PORT_E) & switchNum) !=0);
}

/* set led */
void spartan_edge_ioex::ledSet(unsigned int ledNum) {
  GPIO_SetBits(GPIO_PORT_B, ledNum);
}

/* clear led */
void spartan_edge_ioex::ledClear(unsigned int ledNum) {
  GPIO_ResetBits(GPIO_PORT_B, ledNum);
}

/* led enable */
void spartan_edge_ioex::ledEnable(void) {
  int v;
	
  v = regRead(GPIO_PORT_B);
  GPIO_Init(GPIO_PORT_B, (v | (1 << LED1 | 1 << LED2)));
}

/* led toggle */
void spartan_edge_ioex::ledToggle(unsigned int ledNum) {
  int v;
	
  v = GPIO_ReadOutputData(GPIO_PORT_B);
  GPIO_Write(GPIO_PORT_B, (v ^ (1 << ledNum)));
}







