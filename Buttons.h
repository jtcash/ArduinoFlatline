/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*#############################*/
#ifndef Buttons_h
#define Buttons_h

#include "Arduino.h"
#include "MainMenu.h"
#include "Globals.h" // For PINS and SERIAL_BAUD

/** Class to check for button presses */
class Buttons {
public:
  Buttons() {
    pinMode(PIN_BUTTON_SELECT, INPUT_PULLUP);
    prevSelect = HIGH;
    
    pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
    prevLeft = HIGH;

    pinMode(PIN_BUTTON_RIGHT, INPUT_PULLUP);
    prevRight = HIGH;

    // Set up LED for blinking
    pinMode(PIN_BUTTON_LED_PIN, OUTPUT);
    digitalWrite(PIN_BUTTON_LED_PIN, LOW);

    ledOn = false;
  }
  
  /** Check the buttons to see if they have been pressed */
  void check(){
    bool buttonPressed = false;
    if(digitalRead(PIN_BUTTON_SELECT) == LOW){
      buttonPressed = true;
      if(prevSelect == HIGH){
        prevSelect = LOW;
        menu.selectItem();
      }
    } else {
      prevSelect = HIGH;
    }
    if(digitalRead(PIN_BUTTON_LEFT) == LOW){
      buttonPressed = true;
      if(prevLeft == HIGH){
        prevLeft = LOW;
        menu.scrollUp();
      }
    } else {
      prevLeft = HIGH;
    }
    if(digitalRead(PIN_BUTTON_RIGHT) == LOW){
      buttonPressed = true;
      if(prevRight == HIGH){
        prevRight = LOW;
        menu.scrollDown();
      }
    } else {
      prevRight = HIGH;
    }
    
    if(buttonPressed){
      if(!ledOn){
        digitalWrite(PIN_BUTTON_LED_PIN, HIGH);
        ledOn = true;
      }
    } else {
      if(ledOn){
        digitalWrite(PIN_BUTTON_LED_PIN, LOW);
        ledOn = false;
      }
    }
  }
  
private:
  byte prevSelect;  // The previous value of pin 11
  byte prevLeft;    // The previous value of pin 12
  byte prevRight;   // The previous value of pin 13
  bool ledOn;       // True if the led is on
};



#endif
