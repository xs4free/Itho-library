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
#define ITHO_RFT_DEVIATN 0x40 //26000000 / 131072 * (8 + 0          ) * 2 ^ 5 = 50,78125 KHz

#define CC1150_DEFVAL_IOCFG1     0x2E        // GDO1 Output Pin Configuration
#define CC1150_DEFVAL_IOCFG0     0x06        // GDO0 Output Pin Configuration


const unsigned int ithoPaTable[8] = {0x6F, 0x26, 0x2E, 0x8C, 0x87, 0xCD, 0xC7, 0xC0};

const byte itho_RF_Full[] = {
  170, 170, 170, 173, 51,  83,  74, 203, 76, 205,
  84,  213,  85,  51, 82, 180, 170, 171, 85, 75 };

const byte itho_RFT_Full[] = {
170, 170, 170, 170, 170, 170, 170, 171, 254,   0, 179,
 42, 171,  42, 149, 154, 102,  89, 154, 165, 169, 169,
154,  86, 149, 165, 166,  89, 150, 170, 165, 101,  90,
150,  85, 149, 101,  89, 102,  85, 150, 106, 170, 106,
172, 170, 170, 170, 170, 170 };

class Itho
{
  public:
	void setup(void);
	void begin(void);
	void end(void);
	
	void printRegisters(void);  // print CC1150 register to serial

  private:  
	void printRegister(uint8_t addr, uint8_t registerType, char *name);
};

#endif //_ITHO_H