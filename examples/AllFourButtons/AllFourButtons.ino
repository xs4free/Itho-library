/*
This example shows how to send a full-speed command via a CC1150 chip to an Itho ventilation box
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
               Pin  5        button to send medium command
               Pin  8        button to send low command
               Pin  7        button to send full command
			   Pin  6        button to send timer command
*/

/*
Based on code from:
http://www.electrodragon.com/w/index.php?title=CC1101
https://code.google.com/p/panstamp/downloads/detail?name=panstamp_arduino_1.0.zip&can=2&q=
https://github.com/energia/Energia/tree/master/hardware/msp430/libraries/AIR430BoostUSAFCC/utility
https://github.com/cybergibbons/Response_CC1150
http://arduino.cc/en/Tutorial/Debounce
*/

#include <SPI.h>
#include "itho.h"

const int buttonTimer = 6;     	// the number of the pushbutton pin
const int buttonMedium = 5;    	// the number of the pushbutton pin
const int buttonLow = 8;    	// the number of the pushbutton pin
const int buttonFull = 7;  // the number of the pushbutton pin

int buttonStates[4];             // the current reading from the input pin
int lastButtonStates[4];   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTimes[4];  // the last time the output pin was toggled
long debounceDelay = 15;    // the debounce time; increase if the output flickers

Itho itho;

void setup() {
  Serial.begin(9600);
  Serial.println("setup initializing...");  
  
  itho.setup();  
  
  pinMode(buttonLow, INPUT);
  pinMode(buttonMedium, INPUT);
  pinMode(buttonFull, INPUT);
  pinMode(buttonTimer, INPUT);
  
  buttonStates[0] = buttonStates[1] = buttonStates[2] = buttonStates[3] = LOW;
  lastButtonStates[0] = lastButtonStates[1] = lastButtonStates[2] = lastButtonStates[3] = LOW;
  lastDebounceTimes[0] = lastDebounceTimes[1] = lastDebounceTimes[2] = lastDebounceTimes[3] = 0;
  
  Serial.println("setup done.");
}

void loop() {
  if (readButton(buttonLow, &buttonStates[0], &lastButtonStates[0], &lastDebounceTimes[0]) == HIGH) {
    sendLowSpeed();
  }
  if (readButton(buttonMedium, &buttonStates[1], &lastButtonStates[1], &lastDebounceTimes[1]) == HIGH) {
    sendMediumSpeed();
  }
  if (readButton(buttonFull, &buttonStates[2], &lastButtonStates[2], &lastDebounceTimes[2]) == HIGH) {
    sendFullSpeed();
  }
  if (readButton(buttonTimer, &buttonStates[3], &lastButtonStates[3], &lastDebounceTimes[3]) == HIGH) {
    sendTimer();
  }
}

int readButton(int buttonPin, int* buttonState, int* lastButtonState, long* lastDebounceTime) {
  int resultButtonPressed = LOW;

  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != *lastButtonState) {
    // reset the debouncing timer
    *lastDebounceTime = millis();
  } 
  
  if ((millis() - *lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != *buttonState) {
      *buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (*buttonState == HIGH) {
        resultButtonPressed = HIGH;
      }
    }
  }  

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  *lastButtonState = reading;

  return resultButtonPressed;  
}

void sendRegister() {
   Serial.println("sending join...");
   itho.begin();
   itho.joinItho();
   itho.end();
   Serial.println("sending join done.");
}

void sendLowSpeed() {
   Serial.println("sending low...");
   itho.begin();
   itho.lowSpeed();
   itho.end();
   Serial.println("sending low done.");
}

void sendMediumSpeed() {
   Serial.println("sending medium...");
   itho.begin();
   itho.mediumSpeed();
   itho.end();
   Serial.println("sending medium done.");
}

void sendFullSpeed() {
  Serial.println("sending FullSpeed...");
  itho.begin();
  itho.fullSpeed();
  itho.end();
  Serial.println("sending FullSpeed done.");
}

void sendTimer() {
  Serial.println("sending timer...");
  itho.begin();
  itho.timerSpeed();
  itho.end();
  Serial.println("sending timer done.");
}

/*void blinkLed(int count, int milli) {
  for(int i = 0; i < count; i++) {
    digitalWrite(ledPin, HIGH);  
    delay(milli);
    digitalWrite(ledPin, LOW);
    if (i < count - 1) {
      delay(milli); // make sure the led stays of long enough to see
    }
  }
}*/