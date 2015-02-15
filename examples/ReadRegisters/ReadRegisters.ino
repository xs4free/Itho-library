/*
CC1101 pins    Arduino pins  Description
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
#include <itho.h>

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
  
  itho.printRegisters();
  
  itho.end();
  Serial.println("end.");
}
