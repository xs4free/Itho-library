#ifndef _ITHO_H
#define _ITHO_H

#include <Arduino.h>
#include <SPI.h>

#define ITHO_FREQ2 0x21
#define ITHO_FREQ1 0x65
#define ITHO_FREQ0 0x6A

#define ITHO_CHANNR 	0x00
#define ITHO_DEVIATN	0x40 //26000000 / 131072 * (8 + 0          ) * 2 ^ 4 = 25,390625 KHz

#define ITHO_FREND0	0x17 //LODIV_BUF_CURRENT_TX = 2, PA_POWER = 7
#define ITHO_MCSM0	0x18 //FS_AUTOCAL = Automatically calibrate when going from TX back to IDLE, PO_TIMEOUT = 64
#define ITHO_FSCAL3	0xA9 //FSCAL3 = Reset, CHP_CURR_CAL_EN = Reset, FSCAL3 = Reset
#define ITHO_FSCAL2	0x2A //VCO_CORE_H_EN = high, FSCAL2 = Reset
#define ITHO_FSCAL1	0x00 //FSCAL1 = 0
#define ITHO_FSCAL0	0x11 //FSCAL0 = 17
#define ITHO_FSTEST	0x59 //For test only. Do not write to this register.
#define ITHO_TEST2	0x81 //The value to use in this register is given by the SmartRF Studio [11] software.
#define ITHO_TEST1	0x35 //The value to use in this register is given by the SmartRF Studio [11] software.
#define ITHO_TEST0	0x0B //The value to use in this register is given by the SmartRF Studio [11] software.
#define ITHO_PKTCTRL0	0x12 //WHITE_DATA = off, PKT_FORMAT = "Serial Synchronous mode, data in on GDO0", CRC_EN = disabled, LENGTH_CONFIG = "Infinite packet length packets" 
#define ITHO_ADDR	0x00 //Broadcast
#define ITHO_PKTLEN	0xFF //255
#define ITHO_IOCFG0     0x2D //GDO0_Z_EN_N. When this output is 0, GDO0 is configured as input (for serial TX data)
#define ITHO_IOCFG1     0x0B //Serial Clock. Synchronous to the data in synchronous serial mode. In TX mode, data is sampled by CC1150 on the rising edge of the serial clock when GDOx_INV=0.


//Configuration for RF signal
#define ITHO_RF_MDMCFG4	0x07 //(((256 + 67) * 2 ^ 7) / 2 ^ 28   ) * 26 MHz = 4004,47845458984375 baud
#define ITHO_RF_MDMCFG3 0x43
#define ITHO_RF_MDMCFG2 0x00 //MOD_FORMAT[2:0] = 2-FSK, MANCHESTER_EN[3] = Disable, SYNC_MODE[2:0] = No preamble/sync word
#define ITHO_RF_MDMCFG1 0x22 //FEC_EN = Disable, NUM_PREAMBLE = 4, CHANSPC_E = 2
#define ITHO_RF_MDMCFG0 0xF8 //CHANSPC_M = RESET (248)
#define ITHO_RF_DEVIATN 0x40 //26000000 / 131072 * (8 + 0          ) * 2 ^ 4 = 25,390625 KHz

#define ITHO_RFT1_MDMCFG4 0x08 //(((256 + 67) * 2 ^ 8) / 2 ^ 28   ) * 26 MHz = 8008,9569091796875 baud -> GDO0
#define ITHO_RFT2_MDMCFG4 0x5A //(((256 + 131) * 2 ^ 10) / 2 ^ 28   ) * 26 MHz = 38383,48388671875 baud -> GDO0
#define ITHO_RFT1_MDMCFG3 0x43
#define ITHO_RFT2_MDMCFG3 0x83
#define ITHO_RFT_MDMCFG2 0x00 //MOD_FORMAT[2:0] = 2-FSK, MANCHESTER_EN[3] = Disable, SYNC_MODE[2:0] = No preamble/sync word
#define ITHO_RFT_MDMCFG1 0x22 //FEC_EN = Disable, NUM_PREAMBLE = 4, CHANSPC_E = 2
#define ITHO_RFT_MDMCFG0 0xF8 //CHANSPC_M = RESET (248)
#define ITHO_RFT_DEVIATN 0x50 //26000000 / 131072 * (8 + 0          ) * 2 ^ 5 = 50,78125 KHz

#define CC1150_DEFVAL_IOCFG1     0x2E        // GDO1 Output Pin Configuration
#define CC1150_DEFVAL_IOCFG0     0x06        // GDO0 Output Pin Configuration


//http://forum.arduino.cc/index.php?topic=43687.0
#define SIZEOF_ARRAY(x)  (sizeof (x) / sizeof (x[0]))

const unsigned int ithoPaTable[8] = {0x6F, 0x26, 0x2E, 0x8C, 0x87, 0xCD, 0xC7, 0xC0};

const int itho_RF_Delay = 75; //microseconds
const int itho_RF_Full_BitLength = (20 * 8); 

const int itho_RFT_Delay = 0; //microseconds
const int itho_RFT_Full_BitLength = (50 * 8) + 3; // only send the first 3 bits of the last byte (224 = 1110000)

// Enum for Itho commands
enum ithoCommand {
	low = 1,
	medium = 2,
	full  = 4,
	timer = 8,
	join = 16, //register
	leave = 32 //unregister
};

#define ITHO_REMOTE2

#if defined(ITHO_REMOTE1)
	const byte itho_RF_full_command[] =   { 1,  84, 213, 85,  51,  82, 180};
	const byte itho_RF_medium_command[] = { 1,  84, 213, 85,  75,  76, 180};
	const byte itho_RF_low_command[] =    { 1,  84, 213, 85,  82, 202, 212};
	const byte itho_RF_timer_command[] =  { 1,  83,  83, 84, 205,  83,  52};
	const byte itho_RF_join_command[] =   { 0, 170, 171, 85,  85,  83,  52};
#else
	const byte itho_RF_full_command[] =   { 1,  84, 213, 85,  50, 210, 212};
	const byte itho_RF_medium_command[] = { 1,  84, 213, 85,  74, 204, 212};
	const byte itho_RF_low_command[] =    { 1,  84, 213, 85,  83,  74, 180};
	const byte itho_RF_timer_command[] =  { 1,  83,  83, 84, 204, 211,  84};
	const byte itho_RF_join_command[] =   { 0, 170, 171, 85,  84, 211,  84};
#endif

const byte itho_RF_command_First_BitLength = 1;
const byte itho_RF_command_Last_BitLength = 7;

const byte itho_RFT_full_command[] =   { 38, 89, 150, 170, 165, 101, 90, 150, 85, 149, 101, 89, 102,  85, 150,  6}; //offset 26 * 8 + 2
const byte itho_RFT_medium_command[] = { 38, 89, 150, 170, 165, 101, 90, 150, 85, 149, 101, 90, 150,  85, 150,  6};
const byte itho_RFT_low_command[] =    { 38, 89, 150, 170, 165, 101, 90, 150, 85, 149, 101, 89, 150,  85, 150,  6};
const byte itho_RFT_timer_command[] =  { 38, 89, 150, 170, 169, 101, 90, 150, 85, 149, 101, 89,  86,  85, 153, 10};
const byte itho_RFT_join_command[] =   { 41, 90, 170,  90, 165, 165, 89, 106, 85, 149, 102, 89, 150, 170, 165,  6}; //medium + timer

const byte itho_RFT_command_First_BitLength = 6;
const byte itho_RFT_command_Last_BitLength = 4;

//footers
const byte itho_RFT_regular_footer[] = { 2, 172, 170, 170, 170, 170, 170, 7};

#if defined(ITHO_REMOTE1)
	const byte itho_RFT_join_footer[]    = { 2, 165, 169, 169, 154, 86, 85, 5};
#else
	const byte itho_RFT_join_footer[]    = { 2, 105, 169, 105, 154, 86, 85, 5};
#endif


// remote ids
#if defined(ITHO_REMOTE1)
const byte itho_RF_Id[] =  {  74, 203,  76, 204}; //offset 6 * 8
const byte itho_RFT_Id[] = { 102,  89, 154, 165, 169, 169}; //offset 16 * 8
#else
const byte itho_RF_Id[] =  {  45,  75,  82, 178}; //offset 6 * 8
const byte itho_RFT_Id[] = { 105,  90, 106, 105, 169, 105}; //offset 16 * 8
#endif

const int itho_RF_Id_BitLength = (3 * 8) + 7;
const int itho_RFT_Id_BitLength = (6 * 8);

const byte itho_RF_message[] = { 
	170, 170, 170, 173, 51, 83, //header, 0 - 6
	0, 0, 0, 0, //id, 7 - 10
	0, 0, 0, 0, 0, 0, //command + checksum, 10 - 16
	170, 171, // footer, 17 - 18
	0, 0 //previous command, 19 - 20
	};
	
const byte itho_RFT_message[] = { 
	170, 170, 170, 170, 170, 170, 170, 171, 254, 0, 179, 42, 171, 42, 149, 154, //header
	0, 0, 0, 0, 0, 0, //id
	154, 86, //fixed value
	0, 0, 0,  // counter + commands
	0, 0, 0, 0, 0, 0, 0, 85, 149, 0, 89, 0, 0, 0, //command
	0, 0, 0, //counter
    0, 0, 0, 0, 0, 0, 0 //footer command specific	
	};

class Itho
{
  public:
	void setup(void);
	void begin(void);
	void end(void);

	// command
	void fullSpeed(void);
	void mediumSpeed(void);
	void lowSpeed(void);
	void timerSpeed(void);
	void joinItho(void);
	
	void printRegisters(void);  // print CC1150 register to serial
	
	// for test-purpose only! Will be made private in final version.
	void setupRF();
	void setupRFT();
	void resetGDO();	
  private:  
	void printRegister(uint8_t addr, uint8_t registerType, char *name);
	void printArray(byte* array, int length);
	
	void setupDefaultIthoRegisters();
	
	void sendCommand(ithoCommand command, int rftCounter1, int rftCounter2);	
	byte* getRfCommand(ithoCommand command, ithoCommand previousCommand);
	byte* getRftCommand(ithoCommand command, int counter1, int counter2);
	
	int calculateRftCounter2(int currentRftCounter2, ithoCommand newCommand, ithoCommand previousCommand);
	void setPreviousRfCommand(ithoCommand command, byte* rfCommand);
	void setCounterBit(byte* counter, int index, boolean bitValue);
	void setCounter1Bytes(int value1, byte *byte1, byte *byte2, byte *byte3);
	void setCounter2Bytes(int value1, byte *byte1, byte *byte2, byte *byte3);	
};

#endif //_ITHO_H