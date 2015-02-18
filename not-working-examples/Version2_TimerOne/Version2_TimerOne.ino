/*
This is NOT a working example!!
My Arduino Pro Mini running at 8 MHz was not fast enough to handle all the timer interrupts
*/

/*
CC1150 pins    Arduino pins  Analyzer pin   Description
1 - GND        GND           9              Ground
2 - VCC        VCC           -              3v3
3 - GDO0       Pin2          5              serial data to CC1150
4 - CSN        Pin10         4              Chip select
5 - SCK        Pin13         3              Clock pin
6 - MOSI       Pin11         1              Data input
7 - MISO/GDO1  Pin12         2              Data output / serial clock from CC1150
8 - GDO2       Pin9                         output as a symbol of receiving or sending data
*/

/*
Based on code from:
http://www.electrodragon.com/w/index.php?title=CC1101
https://code.google.com/p/panstamp/downloads/detail?name=panstamp_arduino_1.0.zip&can=2&q=
https://github.com/energia/Energia/tree/master/hardware/msp430/libraries/AIR430BoostUSAFCC/utility
https://github.com/cybergibbons/Response_CC1150
*/


#include <SPI.h>
#include "TimerOne.h"
#include <cc1150.h>
#include <itho.h>

Itho itho;
TimerOne timerOne;

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
  
  timerOne.initialize(26); //26 us = 38.46 kHz
  timerOne.attachInterrupt(ISR_MISO);
}

void disableMisoInterrupt() {
  timerOne.detachInterrupt();

  // clock out the last 3 bits for the RFT full command
  if (outputDelay == 15)
  {
    delayMicroseconds(3);    
    digitalWrite(GDO0, HIGH);
    delayMicroseconds(35); // should be 3 RISING edges
  }

  pinMode(GDO0, INPUT);        // sets the digital pin as output
}
