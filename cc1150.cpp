#include "cc1150.h"

// Send a strobe command
void CC1150::send_command(unsigned int command) {
	select();
	SPI.transfer(command);
	deselect();
}

// Set an individual register. Ignore returned SPI data - we don't care!
void CC1150::writeRegister(unsigned int address, unsigned int data) {
	select();
	SPI.transfer(address);
	SPI.transfer(data);
	deselect();
}

// Used for setting the PA table - this is the only way to do this.
void CC1150::writeBurstRegister(unsigned int address, unsigned int *data, unsigned int length) {
	uint8_t i;
	
	select();
	SPI.transfer(address);
	
	for (i = 0; i < length; i++) {
		SPI.transfer(data[i]);
	}
	
	deselect();
}


unsigned int CC1150::readRegister(unsigned int address, unsigned int registerType) {  	
  unsigned int result;
  unsigned int addr = address | registerType;  
  
  select();
  SPI.transfer(addr);
  result = SPI.transfer(0x00);
  deselect();
  
  return result;
}

// commands
void CC1150::transmit(void){
  send_command(CC1150_STX);
}

void CC1150::idle(void){ 
  send_command(CC1150_SIDLE);
}

void CC1150::powerdown(void) {
  send_command(CC1150_SPWD);
}

// Keeps CC1150 selected and waits for MISO to go low
// as per p21 of datasheet
void CC1150::reset(void) {
	select();
	SPI.transfer(CC1150_SRES);
	waitMiso();
	deselect();
}

// SPI helper functions
inline void CC1150::select(void) {
	digitalWrite(SS, LOW);
}

inline void CC1150::deselect(void) {
	digitalWrite(SS, HIGH);
}

inline void CC1150::waitMiso(void) {
    while(digitalRead(SPI_MISO) == HIGH);
}

void CC1150::begin(void) {
	SPI.begin();
}

void CC1150::end(void) {
	SPI.end();
}

void CC1150::setup(void) {
	delay(100); // wait for the CC1150 to power-up (  
}