// based on code written by @CyberGibbons
// https://github.com/cybergibbons/Response_CC1150

#ifndef _CC1150_H
#define _CC1150_H

#include <Arduino.h>
#include <SPI.h>

#define SPI_SS   10     // PB2 = SPI_SS
#define SPI_MOSI 11     // PB3 = MOSI
#define SPI_MISO 12     // PB4 = MISO
#define SPI_SCK  13     // PB5 = SCK
#define GDO0	 2      // PD2 = INT0

// Control
#define CC1150_IOCFG1       0x01        // GDO1 output pin configuration
#define CC1150_IOCFG0       0x02        // GDO0 output pin configuration
#define CC1150_FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define CC1150_SYNC1        0x04        // Sync word, high byte
#define CC1150_SYNC0        0x05        // Sync word, low byte
#define CC1150_PKTLEN       0x06        // Packet length
#define CC1150_PKTCTRL0     0x08        // Packet automation control
#define CC1150_ADDR			0x09		// Device address
#define CC1150_CHANNR       0x0A        // Channel number
#define CC1150_FREQ2        0x0D        // Frequency control word, high byte
#define CC1150_FREQ1        0x0E        // Frequency control word, middle byte
#define CC1150_FREQ0        0x0F        // Frequency control word, low byte
#define CC1150_MDMCFG4      0x10        // Modem configuration
#define CC1150_MDMCFG3      0x11        // Modem configuration
#define CC1150_MDMCFG2      0x12        // Modem configuration
#define CC1150_MDMCFG1      0x13        // Modem configuration
#define CC1150_MDMCFG0      0x14        // Modem configuration
#define CC1150_DEVIATN      0x15        // Modem deviation setting
#define CC1150_MCSM1        0x17        // Main Radio Control State Machine configuration
#define CC1150_MCSM0        0x18        // Main Radio Control State Machine configuration
#define CC1150_FREND0       0x22        // Front end TX configuration
#define CC1150_FSCAL3       0x23        // Frequency synthesizer calibration
#define CC1150_FSCAL2       0x24        // Frequency synthesizer calibration
#define CC1150_FSCAL1       0x25        // Frequency synthesizer calibration
#define CC1150_FSCAL0       0x26        // Frequency synthesizer calibration
#define CC1150_FSTEST       0x29        // Frequency synthesizer calibration control
#define CC1150_PTEST        0x2A        // Production test
#define CC1150_TEST2        0x2C        // Various test settings
#define CC1150_TEST1        0x2D        // Various test settings
#define CC1150_TEST0        0x2E        // Various test settings

// Strobe commands
#define CC1150_SRES         0x30       // Reset chip.
#define CC1150_SFSTXON      0x31       // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
				       // If in RX/TX: Go to a wait state where only the synthesizer is
				       // running (for quick RX / TX turnaround).
#define CC1150_SXOFF        0x32       // Turn off crystal oscillator.
#define CC1150_SCAL         0x33       // Calibrate frequency synthesizer and turn it off
								// (enables quick start).
#define CC1150_STX          0x35       // In IDLE state: Enable TX. Perform calibration first if
                                       // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                       // Only go to TX if channel is clear.
#define CC1150_SIDLE        0x36       // Exit RX / TX, turn off frequency synthesizer and exit
                                       // Wake-On-Radio mode if applicable.
#define CC1150_SPWD         0x39       // Enter power down mode when CSn goes high.
#define CC1150_SNOP         0x3D       // No operation. May be used to pad strobe commands to two
                                       // bytes for simpler software.
// Status									
#define CC1150_PARTNUM          0x30
#define CC1150_VERSION          0x31
#define CC1150_MARCSTATE        0x35
#define CC1150_PKTSTATUS        0x38
#define CC1150_VCO_VC_DAC       0x39
#define CC1150_TXBYTES          0x3A
#define CC1150_PATABLE          0x3E
#define CC1150_PATABLE_BURST    CC1150_PATABLE + 0x40

#define WRITE_BURST              0x40
#define READ_SINGLE              0x80
#define READ_BURST               0xC0

#define CC1150_CONFIG_REGISTER   READ_SINGLE
#define CC1150_STATUS_REGISTER   READ_BURST

// default values
#define CC1150_FREQ2_DEFAULT    0x1E
#define CC1150_FREQ1_DEFAULT    0xC4
#define CC1150_FREQ0_DEFAULT    0xEC

typedef void (*voidFuncPtr)(void);

class CC1150
{
  public:
	// general setup and loop functions
	void setup(void);
	void begin(void);
	void end(void);
  
	// manipulate registers
	void writeRegister(unsigned int address, unsigned int data);
    void writeBurstRegister(unsigned int address, unsigned int *data, unsigned int length);
    unsigned int readRegister(unsigned int address, unsigned int registerType);
    
	// custom commands
    void send_command(unsigned int command);
	void sendSerialData(byte* data, int numberOfBits, int delay);
	
    // commands
    void transmit(void);
    void idle(void);
    void powerdown(void);
    void reset(void);

  private:  
	// SPI helper functions
    void waitMiso(void);    
    void select(void);
    void deselect(void);
	
	// functions to send serial data
	void ISR_MISO(void);
	inline void enableMisoInterrupt(void);
	void generateFakeInterrupts(void);
	inline void disableMisoInterrupt(void);
	
	// local variables
	int dataLengthBits;
	int delayAfterSerialWrite; //wait X microseconds (us) after changing GDO0
	byte* dataBuffer;		
	volatile int  dataIndexBits;
	volatile byte mask;
	volatile int  bufferIndex;	
};

#endif //_CC1150_H