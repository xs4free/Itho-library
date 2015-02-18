#include <itho.h>
#include <cc1150.h>

CC1150 cc1150;
bool rftSetup; //is the RF or the RFT header send to the CC1150?

void Itho::setup(void) {
	cc1150.setup();
}

void Itho::begin(void) {
	cc1150.begin();
}

void Itho::end(void) {
	cc1150.end();
}

void Itho::fullSpeed(void) {
  for(byte repeatCount = 0; repeatCount < 3; repeatCount++)
  {
      setupRF();
	  cc1150.sendSerialData((byte*)itho_RF_Full, itho_RF_Full_BitLength, itho_RF_Delay);
      resetGDO();
      
      delay(3); //estimate based on traces
      
      setupRFT();
      cc1150.sendSerialData((byte*)itho_RFT_Full, itho_RFT_Full_BitLength, itho_RFT_Delay);
      resetGDO();
      
      delay(60);
  }	
}

/*
Expected output for default CC1150:

Status registers
CC1150_PARTNUM = 2
CC1150_VERSION = 4
CC1150_MARCSTATE = 1
CC1150_PKTSTATUS = 2
CC1150_VCO_VC_DAC = 94
CC1150_TXBYTES = 0

Configuration registers
CC1150_IOCFG1 = 2E
CC1150_IOCFG0 = 3F
CC1150_FIFOTHR = 7
CC1150_SYNC1 = D3
CC1150_SYNC0 = 91
CC1150_PKTLEN = FF
CC1150_PKTCTRL0 = 45
CC1150_CHANNR = 0
CC1150_FREQ2 = 1E
CC1150_FREQ1 = C4
CC1150_FREQ0 = EC
CC1150_MDMCFG4 = C
CC1150_MDMCFG3 = 22
CC1150_MDMCFG2 = 12
CC1150_MDMCFG1 = 22
CC1150_MDMCFG0 = F8
CC1150_DEVIATN = 47
CC1150_MCSM1 = 0
CC1150_MCSM0 = 8
CC1150_FREND0 = 10
CC1150_FSCAL3 = A9
CC1150_FSCAL2 = A
CC1150_FSCAL1 = 20
CC1150_FSCAL0 = D
CC1150_FSTEST = 57
CC1150_PTEST = 7F
CC1150_TEST2 = 0
CC1150_TEST1 = 21
CC1150_TEST0 = B
*/
void Itho::printRegisters() {
	Serial.println("Status registers");
	printRegister(CC1150_PARTNUM, CC1150_STATUS_REGISTER, "CC1150_PARTNUM");
	printRegister(CC1150_VERSION, CC1150_STATUS_REGISTER, "CC1150_VERSION");
	printRegister(CC1150_MARCSTATE, CC1150_STATUS_REGISTER, "CC1150_MARCSTATE");
	printRegister(CC1150_PKTSTATUS, CC1150_STATUS_REGISTER, "CC1150_PKTSTATUS");
	printRegister(CC1150_VCO_VC_DAC, CC1150_STATUS_REGISTER, "CC1150_VCO_VC_DAC");
	printRegister(CC1150_TXBYTES, CC1150_STATUS_REGISTER, "CC1150_TXBYTES");
	Serial.println("");

	Serial.println("Configuration registers");
	printRegister(CC1150_IOCFG1, CC1150_CONFIG_REGISTER, "CC1150_IOCFG1");
	printRegister(CC1150_IOCFG0, CC1150_CONFIG_REGISTER, "CC1150_IOCFG0");
	printRegister(CC1150_FIFOTHR, CC1150_CONFIG_REGISTER, "CC1150_FIFOTHR");
	printRegister(CC1150_SYNC1, CC1150_CONFIG_REGISTER, "CC1150_SYNC1");
	printRegister(CC1150_SYNC0, CC1150_CONFIG_REGISTER, "CC1150_SYNC0");
	printRegister(CC1150_PKTLEN, CC1150_CONFIG_REGISTER, "CC1150_PKTLEN");
	printRegister(CC1150_PKTCTRL0, CC1150_CONFIG_REGISTER, "CC1150_PKTCTRL0");
	printRegister(CC1150_CHANNR, CC1150_CONFIG_REGISTER, "CC1150_CHANNR");
	printRegister(CC1150_FREQ2, CC1150_CONFIG_REGISTER, "CC1150_FREQ2");
	printRegister(CC1150_FREQ1, CC1150_CONFIG_REGISTER, "CC1150_FREQ1");
	printRegister(CC1150_FREQ0, CC1150_CONFIG_REGISTER, "CC1150_FREQ0");
	printRegister(CC1150_MDMCFG4, CC1150_CONFIG_REGISTER, "CC1150_MDMCFG4");
	printRegister(CC1150_MDMCFG3, CC1150_CONFIG_REGISTER, "CC1150_MDMCFG3");
	printRegister(CC1150_MDMCFG2, CC1150_CONFIG_REGISTER, "CC1150_MDMCFG2");
	printRegister(CC1150_MDMCFG1, CC1150_CONFIG_REGISTER, "CC1150_MDMCFG1");
	printRegister(CC1150_MDMCFG0, CC1150_CONFIG_REGISTER, "CC1150_MDMCFG0");
	printRegister(CC1150_DEVIATN, CC1150_CONFIG_REGISTER, "CC1150_DEVIATN");
	printRegister(CC1150_MCSM1, CC1150_CONFIG_REGISTER, "CC1150_MCSM1");
	printRegister(CC1150_MCSM0, CC1150_CONFIG_REGISTER, "CC1150_MCSM0");
	printRegister(CC1150_FREND0, CC1150_CONFIG_REGISTER, "CC1150_FREND0");
	printRegister(CC1150_FSCAL3, CC1150_CONFIG_REGISTER, "CC1150_FSCAL3");
	printRegister(CC1150_FSCAL2, CC1150_CONFIG_REGISTER, "CC1150_FSCAL2");
	printRegister(CC1150_FSCAL1, CC1150_CONFIG_REGISTER, "CC1150_FSCAL1");
	printRegister(CC1150_FSCAL0, CC1150_CONFIG_REGISTER, "CC1150_FSCAL0");
	printRegister(CC1150_FSTEST, CC1150_CONFIG_REGISTER, "CC1150_FSTEST");
	printRegister(CC1150_PTEST, CC1150_CONFIG_REGISTER, "CC1150_PTEST");
	printRegister(CC1150_TEST2, CC1150_CONFIG_REGISTER, "CC1150_TEST2");
	printRegister(CC1150_TEST1, CC1150_CONFIG_REGISTER, "CC1150_TEST1");
	printRegister(CC1150_TEST0, CC1150_CONFIG_REGISTER, "CC1150_TEST0");
}

void Itho::printRegister(uint8_t addr, uint8_t registerType, char *name) {
	Serial.print(name);
	Serial.print(" = ");
	uint8_t val = cc1150.readRegister(addr, registerType);
	Serial.println(val, HEX);
}

void Itho::setupRF() {
  cc1150.reset();
  
  cc1150.writeRegister(CC1150_IOCFG0, CC1150_DEFVAL_IOCFG1); //0x02 0x2E

  cc1150.writeRegister(CC1150_MDMCFG4 ,ITHO_RF_MDMCFG4); //0x10 0x07
  cc1150.writeRegister(CC1150_MDMCFG3 ,ITHO_RF_MDMCFG3); //0x11 0x43
  cc1150.writeRegister(CC1150_MDMCFG2 ,ITHO_RF_MDMCFG2); //0x12 0x00
  cc1150.writeRegister(CC1150_MDMCFG1 ,ITHO_RF_MDMCFG1); //0x13 0x22
  cc1150.writeRegister(CC1150_MDMCFG0 ,ITHO_RF_MDMCFG0); //0x14 0xF8
  
  cc1150.writeRegister(CC1150_CHANNR, ITHO_CHANNR); //0x0A 0x00
  cc1150.writeRegister(CC1150_DEVIATN, ITHO_RF_DEVIATN); //0x15 0x40

  setupDefaultIthoRegisters();

  cc1150.writeRegister(CC1150_MDMCFG4, ITHO_RFT1_MDMCFG4); //0x10 0x08
  cc1150.writeRegister(CC1150_MDMCFG3, ITHO_RFT1_MDMCFG3); //0x11 0x43
  cc1150.writeRegister(CC1150_DEVIATN, ITHO_RF_DEVIATN);  //0x15 0x40

  cc1150.writeRegister(CC1150_IOCFG0, 0x0C); //0x02 Serial Synchronous Data Output
  cc1150.writeRegister(CC1150_IOCFG1, 0x0B); //0x01 Serial clock

  cc1150.transmit(); //0x35
  
  rftSetup = false;
}

void Itho::setupDefaultIthoRegisters() {
  cc1150.writeRegister(CC1150_FREQ2 ,ITHO_FREQ2); //0x0D 0x21
  cc1150.writeRegister(CC1150_FREQ1 ,ITHO_FREQ1); //0x0E 0x65
  cc1150.writeRegister(CC1150_FREQ0 ,ITHO_FREQ0); //0x0F 0x6A
  
  cc1150.writeRegister(CC1150_FREND0, ITHO_FREND0); //0x22 0x17
  cc1150.writeRegister(CC1150_MCSM0, ITHO_MCSM0); //0x18 0x18
  cc1150.writeRegister(CC1150_FSCAL3, ITHO_FSCAL3); //0x23 0xA9
  cc1150.writeRegister(CC1150_FSCAL2, ITHO_FSCAL2); //0x24 0x2A
  cc1150.writeRegister(CC1150_FSCAL1, ITHO_FSCAL1); //0x25 0x00
  cc1150.writeRegister(CC1150_FSCAL0, ITHO_FSCAL0); //0x26 0x11
  cc1150.writeRegister(CC1150_FSTEST, ITHO_FSTEST); //0x29 0x59
  cc1150.writeRegister(CC1150_TEST2, ITHO_TEST2); //0x2C 0x81
  cc1150.writeRegister(CC1150_TEST1, ITHO_TEST1); //0x2D 0x35
  cc1150.writeRegister(CC1150_TEST0, ITHO_TEST0); //0x2E 0x0B
  
  cc1150.writeRegister(CC1150_PKTCTRL0, ITHO_PKTCTRL0); //0x08 0x12
  
  cc1150.writeRegister(CC1150_PKTLEN, ITHO_PKTLEN); //0x06 0xFF
  cc1150.writeBurstRegister(CC1150_PATABLE_BURST, (unsigned int*)ithoPaTable, 8);
  cc1150.idle(); //0x36
  cc1150.idle(); //0x36  
  cc1150.idle(); //0x36
}

void Itho::setupRFT() {
  cc1150.reset();
  cc1150.writeRegister(CC1150_IOCFG0, CC1150_DEFVAL_IOCFG1);
  cc1150.writeRegister(CC1150_MDMCFG4, ITHO_RFT1_MDMCFG4);
  cc1150.writeRegister(CC1150_MDMCFG3, ITHO_RFT1_MDMCFG3);
  cc1150.writeRegister(CC1150_MDMCFG2, ITHO_RFT_MDMCFG2);
  cc1150.writeRegister(CC1150_MDMCFG1, ITHO_RFT_MDMCFG1);
  cc1150.writeRegister(CC1150_MDMCFG0, ITHO_RFT_MDMCFG0);
  cc1150.writeRegister(CC1150_CHANNR, ITHO_CHANNR);
  cc1150.writeRegister(CC1150_DEVIATN, ITHO_RFT_DEVIATN);
  
  setupDefaultIthoRegisters();

  cc1150.writeRegister(CC1150_MDMCFG4, ITHO_RFT2_MDMCFG4);
  cc1150.writeRegister(CC1150_MDMCFG3, ITHO_RFT2_MDMCFG3);
  cc1150.writeRegister(CC1150_DEVIATN, ITHO_RFT_DEVIATN);
  cc1150.writeRegister(CC1150_IOCFG0, ITHO_IOCFG0);
  cc1150.writeRegister(CC1150_IOCFG1, ITHO_IOCFG1);
  cc1150.transmit();
  cc1150.idle();

  cc1150.writeRegister(CC1150_MDMCFG4, ITHO_RFT2_MDMCFG4);
  cc1150.writeRegister(CC1150_MDMCFG3, ITHO_RFT2_MDMCFG3);
  cc1150.writeRegister(CC1150_DEVIATN, ITHO_RFT_DEVIATN);
  cc1150.writeRegister(CC1150_IOCFG0, ITHO_IOCFG0);
  cc1150.writeRegister(CC1150_IOCFG1, ITHO_IOCFG1);
  cc1150.transmit();  
  
  rftSetup = true;
}

void Itho::resetGDO() {
  cc1150.idle();
  cc1150.writeRegister(CC1150_IOCFG0, CC1150_DEFVAL_IOCFG1);
  cc1150.writeRegister(CC1150_IOCFG1, CC1150_DEFVAL_IOCFG1);
  cc1150.idle();
  cc1150.powerdown();
}