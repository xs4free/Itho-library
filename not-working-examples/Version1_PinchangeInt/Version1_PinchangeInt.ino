/*
This is NOT a working example!!
My Arduino Pro Mini running at 8 MHz was not fast enough to handle all the interrupts
*/

/*
CC1150 pins    Arduino pins  Description
1 - GND        GND           Ground
2 - VCC        VCC           3v3
3 - GDO0       Pin  2        Serial data to CC1150
4 - CSN        Pin 10        Chip select
5 - SCK        Pin 13        Clock pin
6 - MOSI       Pin 11        Data input to CC1150
7 - MISO/GDO1  Pin 12        Data output from CC1150 / serial clock from CC1150
8 - GDO2       Pin  9        output as a symbol of receiving or sending data
*/

/*
Based on code from:
http://www.electrodragon.com/w/index.php?title=CC1101
https://code.google.com/p/panstamp/downloads/detail?name=panstamp_arduino_1.0.zip&can=2&q=
https://github.com/energia/Energia/tree/master/hardware/msp430/libraries/AIR430BoostUSAFCC/utility
https://github.com/cybergibbons/Response_CC1150
*/



#include <SPI.h>
#include <cc1150.h>
#include <itho.h>
#include "PinChangeInt.h"

Itho itho;

void setup() {
  Serial.begin(9600);
  Serial.println("setup initializing...");
  
  itho.setup();
  
  Serial.println("setup done.");
}

void loop() {
  Serial.println("sleeping...");  
  delay(5000);
  
  Serial.println("beginning...");
  itho.begin();
  
  for(byte repeatCount = 0; repeatCount < 3; repeatCount++)
  {
      itho.setupRF();
      sendRFButton();
      itho.resetGDO();
      
      delay(3); //estimate based on traces
      
      itho.setupRFT();
      sendRFTButton();
      itho.resetGDO();
      
      delay(60);
  }
  
  itho.end();
  Serial.println("end.");
}


int outputDelay = 17;

void sendRFButton() {
  outputDelay = 110;
  clock_out_data_GDO0((byte*)itho_RF_Full, 20);
}

void sendRFTButton() {
  outputDelay = 15;
  clock_out_data_GDO0((byte*)itho_RFT_Full, 50);
}


int dataLengthBits;
byte* dataBuffer;

volatile int  dataIndexBits = 0;
volatile byte mask = 10000000;
volatile int  bufferIndex = 0;


void clock_out_data_GDO0(byte* data, int length) {
  dataIndexBits = 0;
  mask = 10000000; //reset mask  
  bufferIndex = 0;  
  
  dataLengthBits = length * 8;
  dataBuffer = data;
  
  // listen for clock (interrupt) on MISO
  enableMisoInterrupt();
 
  while(dataIndexBits < dataLengthBits) {
    // wait for the next interrupt to clock out remaining data
  }

  disableMisoInterrupt();
  
  dataBuffer = NULL;
}

//https://github.com/GreyGnome/PinChangeInt
//http://playground.arduino.cc/Main/PinChangeInterrupt
//http://gammon.com.au/interrupts
void ISR_MISO() {
 
  if (dataIndexBits < dataLengthBits) { // check if there is any more data to send
  
    byte bufferValue = dataBuffer[bufferIndex];
    
    if (bufferValue & mask) {
      digitalWrite(GDO0, HIGH);
    }
    else {
      digitalWrite(GDO0, LOW);
    }
    
    mask >>= 1; // bitmask trick came from: http://arduino.cc/en/Tutorial/BitMask
    
    if (mask == 0)
    {
      mask = 10000000; //reset mask
      bufferIndex++; //goto next byte in the data-array
    }
    
    dataIndexBits++; //increase bits send   
  }  
}

void enableMisoInterrupt() {
  pinMode(GDO0, OUTPUT);        // sets the digital pin as output
  
  ISR_MISO(); //queue the first bit, because the code below waits for the first RISING edge to queue the next
  
  PCintPort::attachInterrupt(SPI_MISO, &ISR_MISO, FALLING);
}

void disableMisoInterrupt() {
  PCintPort::detachInterrupt(SPI_MISO);
  
  // clock out the last 3 bits for the RFT full command
  if (outputDelay == 15)
  {
    delayMicroseconds(3);    
    digitalWrite(GDO0, HIGH);
    delayMicroseconds(35); // should be 3 RISING edges
  }  

  pinMode(GDO0, INPUT);        // sets the digital pin as output
}
