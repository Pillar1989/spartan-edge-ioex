#include <spartan-edge-ioex.h>

/* constructors */
spartan_edge_ioex::spartan_edge_ioex(){
	int v;
	
  // release FPGA logic
  digitalWrite(resetPin, LOW);
  
  // set the slaveSelectPin as an output:
  pinMode(slaveSelectPin, OUTPUT);

  // initialize SPI:
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE3));

  // set the reset Pin as an output
  pinMode(resetPin,       OUTPUT);
  
  //set FPGA logic
  digitalWrite(resetPin, HIGH);  
  
  Wire.begin();
}

/* read register */
unsigned spartan_edge_ioex::regRead(int address){
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
void spartan_edge_ioex::GPIO_SetBits(unsigned int addr, unsigned int GPIO_Pin) {
  unsigned int v = 0;
  unsigned int val =1 <<GPIO_Pin;
  
  v = regRead(addr);
  regWrite(addr, v | val);
}

/* GPIO Reset a bit */
void spartan_edge_ioex::GPIO_ResetBits(unsigned int addr, unsigned int GPIO_Pin) {
  unsigned int v = 0;
  unsigned int val =1 <<GPIO_Pin;
  
  v = regRead(addr);
  regWrite(addr, v & ~val);
}

/* GPIO write a bit 1 or 0 */
void spartan_edge_ioex::GPIO_WriteBit(unsigned int addr, unsigned int GPIO_Pin, unsigned int BitVal) {
  unsigned int v = 0;
  unsigned int val =1 << GPIO_Pin;
  
  v = regRead(addr);
  regWrite(addr, (v & ~val) | (BitVal << GPIO_Pin));
}
