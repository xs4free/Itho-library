#include <itho.h>
#include <cc1150.h>

//preparations to get this code working: press a single button on the Itho remote twice before sending any command using the code below

CC1150 cc1150;
bool rftSetup; //is the RF or the RFT header send to the CC1150?
int rftCounter1 = 253; 
int rftCounter2 = 4; 
ithoCommand previousCommand = medium; 

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
	rftCounter2 = calculateRftCounter2(rftCounter2, full, previousCommand);		
	sendCommand(full, rftCounter1, rftCounter2);
	
	rftCounter1--;
}

void Itho::mediumSpeed(void) {
	rftCounter2 = calculateRftCounter2(rftCounter2, medium, previousCommand);		
	sendCommand(medium, rftCounter1, rftCounter2);
	
	rftCounter1--;	
}

void Itho::lowSpeed(void) {
	rftCounter2 = calculateRftCounter2(rftCounter2, low, previousCommand);
	sendCommand(low, rftCounter1, rftCounter2);
	
	rftCounter1--;
}

void Itho::timerSpeed(void) {
	rftCounter2 = calculateRftCounter2(rftCounter2, timer, previousCommand);
	sendCommand(timer, rftCounter1, rftCounter2);
	
	rftCounter1--;	
}

void Itho::joinItho(void) {
	sendCommand(join, rftCounter1, 213); //213 = medium + timer, 226 = low + full
	rftCounter1--;	
}


void Itho::sendCommand(ithoCommand command, int rftCounter1, int rftCounter2) {
  Serial.print("rftCounter1 = ");
  Serial.println(rftCounter1);
  Serial.print("rftCounter2 = ");
  Serial.println(rftCounter2);
  
  byte* rfCommand = getRfCommand(command, previousCommand);
  int rfCommandLength = SIZEOF_ARRAY(itho_RF_message);
  printArray(rfCommand, rfCommandLength);

  byte* rftCommand = getRftCommand(command, rftCounter1, rftCounter2);
  int rftCommandLength = SIZEOF_ARRAY(itho_RFT_message);
  printArray(rftCommand, rftCommandLength);
  
  for(byte repeatCount = 0; repeatCount < 3; repeatCount++)
  {
      setupRF();
	  cc1150.sendSerialData(rfCommand, itho_RF_Full_BitLength, itho_RF_Delay);
      resetGDO();
      
      delay(3); //estimate based on traces
      
      setupRFT();
	  cc1150.sendSerialData(rftCommand, itho_RFT_Full_BitLength, itho_RFT_Delay);
      resetGDO();
      
      delay(40);
  }		
  
  delete rfCommand;
  delete rftCommand;
  
  previousCommand = command;
}


void Itho::printArray(byte* array, int length) {
	for (int i = 0; i < length; i++) {
		Serial.print(array[i]);
		if (i < length - 1) {
			Serial.print(",");
		}
	}
	Serial.println("");
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

  //cc1150.writeRegister(CC1150_IOCFG0, 0x0C); //0x02 Serial Synchronous Data Output
  cc1150.writeRegister(CC1150_IOCFG0, ITHO_IOCFG0); //0x02 0x2D GDO0_Z_EN_N. When this output is 0, GDO0 is configured as input (for serial TX data)  
  cc1150.writeRegister(CC1150_IOCFG1, ITHO_IOCFG1); //0x01 0x0B Serial clock

  cc1150.transmit(); //0x35
  
  // added extra duplicate (?) code
  cc1150.idle(); //0x36
  cc1150.idle(); //0x36

  cc1150.writeRegister(CC1150_MDMCFG4 ,0x08); //0x10 0x08
  cc1150.writeRegister(CC1150_MDMCFG3 ,ITHO_RF_MDMCFG3); //0x11 0x43
  cc1150.writeRegister(CC1150_DEVIATN, ITHO_RF_DEVIATN); //0x15 0x40

  cc1150.writeRegister(CC1150_IOCFG0, ITHO_IOCFG0); //0x02 0x2D GDO0_Z_EN_N. When this output is 0, GDO0 is configured as input (for serial TX data)  
  cc1150.writeRegister(CC1150_IOCFG1, ITHO_IOCFG1); //0x01 0x0B Serial clock
  
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
  cc1150.writeRegister(CC1150_ADDR, ITHO_ADDR); //0x09 0x00
  
  cc1150.writeRegister(CC1150_PKTLEN, ITHO_PKTLEN); //0x06 0xFF
  cc1150.writeBurstRegister(CC1150_PATABLE_BURST, (unsigned int*)ithoPaTable, 8); //0x7E
  cc1150.idle(); //0x36
  cc1150.idle(); //0x36  
  cc1150.idle(); //0x36
}

void Itho::setupRFT() {
  cc1150.reset(); //0x30
  cc1150.writeRegister(CC1150_IOCFG0, CC1150_DEFVAL_IOCFG1); //0x02 0x2E
  cc1150.writeRegister(CC1150_MDMCFG4, ITHO_RFT2_MDMCFG4); //0x10 0x5A
  cc1150.writeRegister(CC1150_MDMCFG3, ITHO_RFT2_MDMCFG3); //0x11 0x83
  cc1150.writeRegister(CC1150_MDMCFG2, ITHO_RFT_MDMCFG2); //0x12 0x00
  cc1150.writeRegister(CC1150_MDMCFG1, ITHO_RFT_MDMCFG1); //0x13 0x22
  cc1150.writeRegister(CC1150_MDMCFG0, ITHO_RFT_MDMCFG0); //0x14 0xF8
  cc1150.writeRegister(CC1150_CHANNR, ITHO_CHANNR); //0x0A 0x00
  cc1150.writeRegister(CC1150_DEVIATN, ITHO_RFT_DEVIATN); //0x15 0x50
  
  setupDefaultIthoRegisters();

  cc1150.writeRegister(CC1150_MDMCFG4, ITHO_RFT2_MDMCFG4); //0x10 0x5A
  cc1150.writeRegister(CC1150_MDMCFG3, ITHO_RFT2_MDMCFG3); //0x11 0x83
  cc1150.writeRegister(CC1150_DEVIATN, ITHO_RFT_DEVIATN); //0x15 0x50
  cc1150.writeRegister(CC1150_IOCFG0, ITHO_IOCFG0); //0x02 0x2D GDO0_Z_EN_N. When this output is 0, GDO0 is configured as input (for serial TX data)  
  cc1150.writeRegister(CC1150_IOCFG1, ITHO_IOCFG1); //0x01 0x0B Serial clock
  cc1150.transmit(); //0x35
  cc1150.idle(); //0x36

  cc1150.writeRegister(CC1150_MDMCFG4, ITHO_RFT2_MDMCFG4); //0x10 0x5A
  cc1150.writeRegister(CC1150_MDMCFG3, ITHO_RFT2_MDMCFG3); //0x11 0x83
  cc1150.writeRegister(CC1150_DEVIATN, ITHO_RFT_DEVIATN); //0x15 0x50
  cc1150.writeRegister(CC1150_IOCFG0, ITHO_IOCFG0); //0x02 0x2D GDO0_Z_EN_N. When this output is 0, GDO0 is configured as input (for serial TX data)  
  cc1150.writeRegister(CC1150_IOCFG1, ITHO_IOCFG1); //0x01 0x0B Serial clock
  cc1150.transmit();  
  
  rftSetup = true;
}

void Itho::resetGDO() {
  cc1150.idle(); //0x36
  cc1150.writeRegister(CC1150_IOCFG0, CC1150_DEFVAL_IOCFG1); //0x02 0x2E
  cc1150.writeRegister(CC1150_IOCFG1, CC1150_DEFVAL_IOCFG1); //0x01 0x2E
  cc1150.idle(); //0x36
  cc1150.powerdown(); //0x39
}

byte* Itho::getRfCommand(ithoCommand command, ithoCommand previousCommand)
{
	// create an empty results message
	int size = SIZEOF_ARRAY(itho_RF_message);
	byte* result = new byte[size];
	memcpy(result, itho_RF_message, size);
	
	// set the id of the remote
	memcpy(&result[6], itho_RF_Id, SIZEOF_ARRAY(itho_RF_Id));
	
	// select command
	const byte *commandBytes;
	switch(command)
	{
		default:
		case full: commandBytes = &itho_RF_full_command[0]; break;
		case medium: commandBytes = &itho_RF_medium_command[0]; break;
		case low: commandBytes = &itho_RF_low_command[0]; break;
		case timer: commandBytes = &itho_RF_timer_command[0]; break;
		case join: commandBytes = &itho_RF_join_command[0]; break;
		//case leave: commandBytes = &itho_RF_full_command[0]; break;
	}
	
	// set the first byte of the command
	result[9] |= commandBytes[0];
	
	// set the rest of the command
	size_t	commandBytesToCopy = SIZEOF_ARRAY(itho_RF_full_command) - 1;
	memcpy(&result[10], &commandBytes[1], commandBytesToCopy);
	
	setPreviousRfCommand(previousCommand, result);

	return result;
}

void Itho::setPreviousRfCommand(ithoCommand previousCommand, byte* rfCommand)
{
	// set the bytes in the rfCommand based on the previous command
	switch(previousCommand)
	{
		default:
		case full: 
			rfCommand[18] = 85;
			rfCommand[19] = 53;
			break;
		case medium:
			rfCommand[18] = 85;
			rfCommand[19] = 75;
			break;
		case low:
			rfCommand[18] = 85;
			rfCommand[19] = 77; 
			break;
		case join:
			rfCommand[18] = 77;
			rfCommand[19] = 77;
			break;
		case leave: //TODO: check if this is correct
			rfCommand[18] = 82;
			rfCommand[19] = 171;
			break;
		case timer: 
			rfCommand[18] = 85;
			rfCommand[19] = 85;
			break;
	}	
}


byte* Itho::getRftCommand(ithoCommand command, int counter1, int counter2)
{
	// create an empty results message
	int size = SIZEOF_ARRAY(itho_RFT_message);
	byte* result = new byte[size];
	memcpy(result, itho_RFT_message, size);
	
	// set the id of the remote
	memcpy(&result[16], itho_RFT_Id, SIZEOF_ARRAY(itho_RFT_Id));
	
	// set the counter 1 bytes
	setCounter1Bytes(counter1, &result[24], &result[25], &result[26]);
	
	// select command
	const byte *commandBytes;
	switch(command)
	{
		default:
		case full: commandBytes = &itho_RFT_full_command[0]; break;
		case medium: commandBytes = &itho_RFT_medium_command[0]; break;
		case low: commandBytes = &itho_RFT_low_command[0]; break;
		case timer: commandBytes = &itho_RFT_timer_command[0]; break;
		case join: commandBytes = &itho_RFT_join_command[0]; break;
		//case leave: commandBytes = &itho_RFT_full_command[0]; break;
	}
	
	// set the first byte of the command
	result[26] |= commandBytes[0];

	// set the rest of the command
	size_t	commandBytesToCopy = SIZEOF_ARRAY(itho_RFT_full_command) - 1;
	memcpy(&result[27], &commandBytes[1], commandBytesToCopy);
	
	// correct the bit position of the last command byte
	result[41] <<= 4;
	
	// set the counter 2 bytes
	setCounter2Bytes(counter2, &result[41], &result[42], &result[43]);
	
	//set footer based on command
	switch(command)
	{
		default:
		case full:
		case medium:
		case low:
		case timer: 
			result[43] |= itho_RFT_regular_footer[0];
			memcpy(&result[44], &itho_RFT_regular_footer[1], SIZEOF_ARRAY(itho_RFT_regular_footer) - 1);
			break;
		case join:
		case leave: 
			result[43] |= itho_RFT_join_footer[0];
			memcpy(&result[44], &itho_RFT_join_footer[1], SIZEOF_ARRAY(itho_RFT_join_footer) - 1);
			break;
	}
	
	// correct the bit position of the last footer byte
	result[50] <<= 5;	

	return result;	
}

int Itho::calculateRftCounter2(int currentRftCounter2, ithoCommand newCommand, ithoCommand previousCommand) {
	int result = currentRftCounter2;
	
	if (previousCommand == newCommand) {
		result = currentRftCounter2 + 1;
	}
	else {
		switch(previousCommand)
		{
			case low:
				switch(newCommand)
				{
					case medium: result = currentRftCounter2 + 2; break;
					case full: result = currentRftCounter2 + 3; break; 
					case timer: result = currentRftCounter2 + 7; break; 
				}
				break;
			case medium:
				switch(newCommand)
				{
					case low: result = currentRftCounter2 + 0; break;
					case full: result = currentRftCounter2 + 2; break;
					case timer: result = currentRftCounter2 + 6; break;
				}
				break;
			case full:
				switch(newCommand)
				{
					case low: result = currentRftCounter2 - 1; break;
					case medium: result = currentRftCounter2 + 0; break; 
					case timer: result = currentRftCounter2 + 5; break;
				}
				break;
			case timer:
				switch(newCommand)
				{
					case low: result = currentRftCounter2 - 5; break; 
					case medium: result = currentRftCounter2 - 4; break;
					case full: result = currentRftCounter2 - 3; break;
				}
				break;
		}		
	}
	
	return result;
}

void Itho::setCounterBit(byte* counter, int index, boolean bitValue)
{
	if (bitValue)
	{
		*counter |= (1 << index);
	}
}

void Itho::setCounter1Bytes(int value1, byte *byte1, byte *byte2, byte *byte3)
{
	if (value1 > 511)
		return;
	
	int value2 = 511 - value1;
	
	setCounterBit(byte1, 7, (16 & value2));
	setCounterBit(byte1, 6, (16 & value1));	
	setCounterBit(byte1, 5, (32 & value2));
	setCounterBit(byte1, 4, (32 & value1));		
	setCounterBit(byte1, 3, (64 & value2));
	setCounterBit(byte1, 2, (64 & value1));		
	setCounterBit(byte1, 1, (128 & value2));
	setCounterBit(byte1, 0, (128 & value1));		
	
	setCounterBit(byte2, 7, (256 & value2));
	setCounterBit(byte2, 6, (256 & value1));	
	setCounterBit(byte2, 5, (1 & value2));
	setCounterBit(byte2, 4, (1 & value1));	
	setCounterBit(byte2, 3, (2 & value2));
	setCounterBit(byte2, 2, (2 & value1));	
	setCounterBit(byte2, 1, (4 & value2));
	setCounterBit(byte2, 0, (4 & value1));	
	
	setCounterBit(byte3, 7, (8 & value2));
	setCounterBit(byte3, 6, (8 & value1));	
}

void Itho::setCounter2Bytes(int value1, byte *byte1, byte *byte2, byte *byte3)
{
	if (value1 > 511)
		return;
	
	int value2 = 511 - value1;
	
	setCounterBit(byte1, 3, (16 & value2));
	setCounterBit(byte1, 2, (16 & value1));		
	setCounterBit(byte1, 1, (32 & value2));
	setCounterBit(byte1, 0, (32 & value1));		
	
	setCounterBit(byte2, 7, (64 & value2));
	setCounterBit(byte2, 6, (64 & value1));	
	setCounterBit(byte2, 5, (128 & value2));
	setCounterBit(byte2, 4, (128 & value1));	
	setCounterBit(byte2, 3, (256 & value2));
	setCounterBit(byte2, 2, (256 & value1));	
	setCounterBit(byte2, 1, (1 & value2));
	setCounterBit(byte2, 0, (1 & value1));	
	
	setCounterBit(byte3, 7, (2 & value2));
	setCounterBit(byte3, 6, (2 & value1));	
	setCounterBit(byte3, 5, (4 & value2));
	setCounterBit(byte3, 4, (4 & value1));	
	setCounterBit(byte3, 3, (8 & value2));
	setCounterBit(byte3, 2, (8 & value1));	
}

