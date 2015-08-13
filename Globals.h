/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*#############################*/

#ifndef Globals_h
#define Globals_h

#include "Arduino.h"

//// IO Pins ////
#define PIN_BELT_LED 3
// Buttons.h
#define PIN_BUTTON_SELECT 6
#define PIN_BUTTON_LEFT 7
#define PIN_BUTTON_RIGHT 8
#define PIN_BUTTON_LED_PIN 4


// Interrupt number corresponding to a pin
#define PIN_2_INTERRUPT 0
#define PIN_3_INTERRUPT 1
#define PIN_21_INTERRUPT 2
#define PIN_20_INTERRUPT 3
#define PIN_19_INTERRUPT 4
#define PIN_18_INTERRUPT 5


#define SERIAL_IS_ACTIVE true

#if SERIAL_IS_ACTIVE
  #define SERIAL_BAUD 230400 //115200
#endif

#if SERIAL_IS_ACTIVE
  #define PRINT_VAL(x, printName) if(printName){ Serial.print(#x); Serial.print(F(" = ")); }\
  Serial.print(x); Serial.print(F("\t"))
#endif


#define SMOOTHER_DEFAULT_SIZE 8

struct ImperialLength{
  unsigned long feet;
  byte inches;
  //ImperialLength() : feet(0), inches(0) { }
};

class Smoother{
public:
  Smoother(){
    size = SMOOTHER_DEFAULT_SIZE;
    arr = new int[size];
    clear();
  }
  Smoother(byte smootherSize){
    size = smootherSize;
    arr = new int[size];
    clear();
  }
  ~Smoother(){
    delete[] arr;
  }
  void insert(int x){
    if(startup){
      --startup;
      arrSum += x;
    } else {
      arrSum += x - *curPtr;
    }
    *curPtr++ = x;
    if(curPtr == endPtr)
      curPtr = arr;
  }
  inline int getValue(){
    if(startup)
      return 0;
    return arrSum / size;
  }

  inline byte getSize(){
    return size;
  }

  void clear(){
    startup = size;
    curPtr = arr;
    endPtr = arr + size;
    arrSum = 0;
    memset(arr, 0, sizeof(int)*size);
  }
private:
  byte size; 
  byte startup;

  int *arr;
  int *curPtr;
  const int *endPtr;

  long arrSum;
};


class TimeDifferential {
public:
  TimeDifferential() : prevX(0), prevV(0) { }
  void insert(unsigned long x){
    if(prevX != 0)
      prevV = x - prevX;
    prevX = x;
  }
  inline unsigned long getX(){ return prevX; }
  inline unsigned long getV(){ return prevV; }
private:
  unsigned long prevX, prevV;
};


class Differential {
public:
  Differential() : prevX(0), prevV(0) { }
  void insert(int x){
    if(prevX != 0)
      prevV = x - prevX;
    prevX = x;
  }
  inline int getX(){ return prevX; }
  inline int getV(){ return prevV; }
private:
  int prevX, prevV;
};

#endif
