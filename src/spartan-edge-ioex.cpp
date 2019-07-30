#include <spartan-edge-ioex.h>

/* constructors */
spartan_edge_ioex::spartan_edge_ioex(){
  int v;
  
  // set the slaveSelectPin as an output:
  pinMode(slaveSelectPin, OUTPUT);

  // initialize SPI:
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));

  // set the reset Pin as an output
  pinMode(resetPin,       OUTPUT);
  
   // set FPGA logic
  digitalWrite(resetPin, LOW);
  //release FPGA logic
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

/* Initialize the input-output */
void spartan_edge_ioex::GPIO_Init(unsigned int gpioN, unsigned int val) {
  unsigned int v = 0;
  
  v = regRead(gpioN);
  regWrite(gpioN, v | val);
}

/* read a bit form port input data */
unsigned int spartan_edge_ioex::GPIO_ReadInputDataBit(unsigned int gpioN, unsigned int GPIO_Pin) {
  unsigned int v = 0;
  
  v = regRead(gpioN + 2);
  return ((v >> GPIO_Pin) & 0x01);
}

/* read port input data */
unsigned int spartan_edge_ioex::GPIO_ReadInputData(unsigned int gpioN) {
  unsigned int v = 0;
  
  v = regRead(gpioN + 2);
  return v;
}

/* read a bit form port output data */
unsigned int spartan_edge_ioex::GPIO_ReadOutputDataBit(unsigned int gpioN, unsigned int GPIO_Pin) {
  unsigned int v = 0;
  
  v = regRead(gpioN + 1);
  return ((v >> GPIO_Pin) & 0x01);
}

/* read port output data */
unsigned int spartan_edge_ioex::GPIO_ReadOutputData(unsigned int gpioN) {
  int v = 0;
  
  v = regRead(gpioN + 1);
  return v;
}

/* GPIO set a bit */
void spartan_edge_ioex::GPIO_SetBits(unsigned int gpioN, unsigned int GPIO_Pin) {
  unsigned int v = 0;
  unsigned int val = 1 << GPIO_Pin;
  
  v = regRead(gpioN);
  regWrite(gpioN, v | val);
}

/* GPIO Reset a bit */
void spartan_edge_ioex::GPIO_ResetBits(unsigned int gpioN, unsigned int GPIO_Pin) {
  unsigned int v = 0;
  unsigned int val = 1 << GPIO_Pin;
  
  v = regRead(gpioN);
  regWrite(gpioN, v & ~val);
}

/* GPIO write a bit 1 or 0 */
void spartan_edge_ioex::GPIO_WriteBit(unsigned int gpioN, unsigned int GPIO_Pin, unsigned int BitVal) {
  unsigned int v = 0;
  unsigned int val = 1 << GPIO_Pin;
  
  v = regRead(gpioN);
  regWrite(gpioN, (v & ~val) | (BitVal << GPIO_Pin));
}

/*
 * cantrol RGBled color
 * index should be set 0 or 1 
 * red/green/blue should be in the range of 0 to 255
 */
void spartan_edge_ioex::setRGBLedVal(unsigned int index, unsigned int red, unsigned int green, unsigned int blue) {

  if(0 == index) {
	regWrite(SK6805_CTRL, 0x0);
	regWrite(SK6805_DATA, blue);  // blue
	regWrite(SK6805_CTRL, 0x1);
	regWrite(SK6805_DATA, red);   // red
	regWrite(SK6805_CTRL, 0x2);
	regWrite(SK6805_DATA, green); // green
  }
  else if(1 == index) {
	regWrite(SK6805_CTRL, 0x3);
	regWrite(SK6805_DATA, blue);  // blue
	regWrite(SK6805_CTRL, 0x4);
	regWrite(SK6805_DATA, red);   // red
	regWrite(SK6805_CTRL, 0x5);
	regWrite(SK6805_DATA, green); // green
  }
}

/* read ADC_data and return Voltage */
unsigned long /* Voltage(mv) */spartan_edge_ioex::readAdcData(void){
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
void spartan_edge_ioex::writeDacData(unsigned int voltVal/* Voltage(mv) */){
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