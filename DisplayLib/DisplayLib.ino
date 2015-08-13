#include <LiquidCrystal.h>
#include "LCDisplay.h"


LiquidCrystal lcd(22, 23, 24, 25, 26, 27);
LCDisplay display(22, 23, 24, 25, 26, 27);

#define SERIAL_BAUD 230400

#define NUM_TESTS 9   // The number of unique tests
#define REPS 1000     // The number of reps per test
#define TEST_MAX_RANDOM 10000  // The max random value

unsigned long testSimplePrint_P(){
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
    
    display.println_P(0, PSTR("TEST"));
    display.println_P(1, PSTR("TSETEST"));

    
  }
  return millis() - t0;
}
unsigned long testSimplePrint(){
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
  
    display.println(0, "TEST");
    display.println(1, "TSETEST");
    
  }
  return millis() - t0;
}
unsigned long testLongPrint(){
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
  
    display.println(0, "0123456789012345");
    display.println(1, "5432109876543210");
    
  }
  return millis() - t0;
}
unsigned long testLongPrint_P(){
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
  
    display.println_P(0, PSTR("0123456789012345"));
    display.println_P(1, PSTR("5432109876543210"));
    
  }
  return millis() - t0;
}


unsigned long testCompoundPrint_P(){
  unsigned long t0 = millis();
  unsigned int i;
  int int0 = 10;
  int int1 = 9876;
  for(i=0; i<REPS; ++i){

//    TEST_MAX_RANDOM
    size_t bufi;
    bufi = display.fillLineBuff_P(PSTR("integer0="));
    bufi = display.fillLineBuff(bufi, int0);
    display.terminateLineBuff(bufi);
    display.printLineBuff(0);

    bufi = display.fillLineBuff_P(PSTR("integer1="));
    bufi = display.fillLineBuff(bufi, int1);
    display.terminateLineBuff(bufi);
    display.printLineBuff(1);
    
  }
  return millis() - t0;
}
unsigned long testCompoundPrint(){
  unsigned long t0 = millis();
  unsigned int i;
  int int0 = 10;
  int int1 = 9876;
  for(i=0; i<REPS; ++i){

    size_t bufi;
    bufi = display.fillLineBuff("integer0=");
    bufi = display.fillLineBuff(bufi, int0);
    display.terminateLineBuff(bufi);
    display.printLineBuff(0);

    bufi = display.fillLineBuff("integer1=");
    bufi = display.fillLineBuff(bufi, int1);
    display.terminateLineBuff(bufi);
    display.printLineBuff(1);
    
  }
  return millis() - t0;
}


unsigned long testCompoundRand_P(){
  randomSeed(0);
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){

//    TEST_MAX_RANDOM
    size_t bufi;
    bufi = display.fillLineBuff_P(PSTR("Rand0="));
    bufi = display.fillLineBuff(bufi, random(TEST_MAX_RANDOM));
    display.terminateLineBuff(bufi);
    display.printLineBuff(0);

    bufi = display.fillLineBuff_P(PSTR("Rand1="));
    bufi = display.fillLineBuff(bufi, random(TEST_MAX_RANDOM));
    display.terminateLineBuff(bufi);
    display.printLineBuff(1);
    
  }
  return millis() - t0;
}
unsigned long testCompoundRand(){
  randomSeed(0);
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){

    size_t bufi;
    bufi = display.fillLineBuff("Rand0=");
    bufi = display.fillLineBuff(bufi, random(TEST_MAX_RANDOM));
    display.terminateLineBuff(bufi);
    display.printLineBuff(0);

    bufi = display.fillLineBuff("Rand1=");
    bufi = display.fillLineBuff(bufi, random(TEST_MAX_RANDOM));
    display.terminateLineBuff(bufi);
    display.printLineBuff(1);
    
  }
  return millis() - t0;
}
unsigned long testFilledRandom(){
  randomSeed(0);
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){

    size_t bufi;
    bufi = display.fillLineBuff(0, random(TEST_MAX_RANDOM));
    bufi = display.fillLineBuff(bufi, random(TEST_MAX_RANDOM));
    bufi = display.fillLineBuff(bufi, random(TEST_MAX_RANDOM));
    bufi = display.fillLineBuff(bufi, random(TEST_MAX_RANDOM));
    display.terminateLineBuff(bufi);
    display.printLineBuff(0);

    bufi = display.fillLineBuff(0, random(TEST_MAX_RANDOM));
    bufi = display.fillLineBuff(bufi, random(TEST_MAX_RANDOM));
    bufi = display.fillLineBuff(bufi, random(TEST_MAX_RANDOM));
    bufi = display.fillLineBuff(bufi, random(TEST_MAX_RANDOM));
    display.terminateLineBuff(bufi);
    display.printLineBuff(1);
    
  }
  return millis() - t0;
}


unsigned long testDefSimplePrint(){
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
    
    lcd.clear();
    lcd.print("TEST");
    lcd.setCursor(0,1);
    lcd.print("TSETEST");
    
  }
  return millis() - t0;
}
unsigned long testDefSimplePrint_P(){
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
    
    lcd.clear();
    lcd.print(F("TEST"));
    lcd.setCursor(0,1);
    lcd.print(F("TSETEST"));
    
  }
  return millis() - t0;
}
unsigned long testDefLongPrint(){
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
    
    lcd.clear();
    //lcd.setCursor(0,0);
    lcd.print("0123456789012345");
    lcd.setCursor(0,1);
    lcd.print("5432109876543210");

    
  }
  return millis() - t0;
}
unsigned long testDefLongPrint_P(){
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
    
    lcd.clear();
    //lcd.setCursor(0,0);
    lcd.print(F("0123456789012345"));
    lcd.setCursor(0,1);
    lcd.print(F("5432109876543210"));

    
  }
  return millis() - t0;
}

unsigned long testDefCompound(){
  unsigned long t0 = millis();
  unsigned int i;
  int int0 = 10;
  int int1 = 9876;
  for(i=0; i<REPS; ++i){
    
    lcd.clear();
    lcd.print("integer0=");
    lcd.print(int0);
    lcd.setCursor(0,1);
    lcd.print("integer1=");
    lcd.print(int1);
    
  }
  return millis() - t0;
}
unsigned long testDefCompound_P(){
  unsigned long t0 = millis();
  unsigned int i;
  int int0 = 10;
  int int1 = 9876;
  for(i=0; i<REPS; ++i){
    
    lcd.clear();
    lcd.print(F("integer0="));
    lcd.print(int0);
    lcd.setCursor(0,1);
    lcd.print(F("integer1="));
    lcd.print(int1);
    
  }
  return millis() - t0;
}
unsigned long testDefRandom(){
  randomSeed(0);
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
    
    lcd.clear();
    lcd.print("Rand0=");
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.setCursor(0,1);
    lcd.print("Rand1=");
    lcd.print(random(TEST_MAX_RANDOM));
    
  }
  return millis() - t0;
}
unsigned long testDefRandom_P(){
  randomSeed(0);
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
    
    lcd.clear();
    lcd.print(F("Rand0="));
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.setCursor(0,1);
    lcd.print(F("Rand1="));
    lcd.print(random(TEST_MAX_RANDOM));
    
  }
  return millis() - t0;
}

unsigned long testDefFilledRand(){
  randomSeed(0);
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
    
    lcd.clear();
   
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    
    lcd.setCursor(0,1);

    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    
  }
  return millis() - t0;
}
unsigned long testDefFilledRandNoClear(){
  randomSeed(0);
  unsigned long t0 = millis();
  unsigned int i;
  for(i=0; i<REPS; ++i){
    
    lcd.setCursor(0,0);
   
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    
    lcd.setCursor(0,1);

    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    lcd.print(random(TEST_MAX_RANDOM));
    
  }
  return millis() - t0;
}
unsigned long testDummyRandom(){
  randomSeed(0);
  unsigned long t0 = millis();
  unsigned int i;
  unsigned long temp;
  for(i=0; i<REPS; ++i){
    temp += random(TEST_MAX_RANDOM);
    temp += random(TEST_MAX_RANDOM);
    
  }
  return millis() - t0;
}

#define DO_TEST(x, r) Serial.print('\t'); Serial.print(#x); Serial.print(F(" \t= ")); \
r = x; Serial.print(r); Serial.print(F("ms;  \t")); Serial.print((float)r/(float)REPS); \
Serial.println(F("ms/screen refresh"));

void testSpeedFull(){
  
  Serial.print(REPS);
  Serial.println(F(" display updates per test; starting speed tests..."));

  Serial.println(F("Testing LCDisplay.h:"));
  unsigned long custResults[NUM_TESTS];
  unsigned long defResults[NUM_TESTS];
  unsigned long res;
  
  display.begin(16,2);
  display.clear();
  DO_TEST(testSimplePrint(), res);
  custResults[0] = res;
  DO_TEST(testSimplePrint_P(), res);
  custResults[1] = res;
  Serial.print('\n');
  

  
  DO_TEST(testLongPrint(), res);
  custResults[2] = res;
  DO_TEST(testLongPrint_P(), res);
  custResults[3] = res;
  Serial.print('\n');
  
  //results[0] = res;
  
  DO_TEST(testCompoundPrint(), res);
  custResults[4] = res;
  DO_TEST(testCompoundPrint_P(), res);
  custResults[5] = res;
  Serial.print('\n');

  
  DO_TEST(testCompoundRand(), res);
  custResults[6] = res;
  DO_TEST(testCompoundRand_P(), res);
  custResults[7] = res;
  Serial.print('\n');

  DO_TEST(testFilledRandom(), res);
  custResults[8] = res;
  

  
  Serial.println(F("\nTesting LiquidCrystal.h:"));
  lcd.begin(16,2);
  lcd.clear();

  DO_TEST(testDefSimplePrint(), res);
  defResults[0] = res;
  DO_TEST(testDefSimplePrint_P(), res);
  defResults[1] = res;
  Serial.print('\n');
  
  DO_TEST(testDefLongPrint(), res);
  defResults[2] = res;
  DO_TEST(testDefLongPrint_P(), res);
  defResults[3] = res;
  Serial.print('\n');
  
  DO_TEST(testDefCompound(), res);
  defResults[4] = res;
  DO_TEST(testDefCompound_P(), res);
  defResults[5] = res;
  Serial.print('\n');
  
  DO_TEST(testDefRandom(), res);
  defResults[6] = res;
  DO_TEST(testDefRandom_P(), res);
  defResults[7] = res;
  Serial.print('\n');
  
  DO_TEST(testDefFilledRand(), res);
  defResults[8] = res;
  
  Serial.println(F("\nDone.\n"));

  Serial.println(F("LCDisplay.h vs only LiquidCrystal.h:"));
  
#define PRINT_LINE(i) Serial.print(F("():      \t")); Serial.print(defResults[i]); \
Serial.print('\t'); Serial.print(custResults[i]); Serial.print(F("\t Speedup: ")); \
Serial.print((float)defResults[i]/(float)custResults[i]); Serial.println('x')

  Serial.print('\n');
  Serial.print(F("SimplePrint"));
  PRINT_LINE(0);
  Serial.print(F("SimplePrint_P"));
  PRINT_LINE(1);
  
  Serial.print('\n');
  Serial.print(F("LongPrint"));
  PRINT_LINE(2);
  Serial.print(F("LongPrint_P"));
  PRINT_LINE(3);

  Serial.print('\n');
  Serial.print(F("Compound"));
  PRINT_LINE(4);
  Serial.print(F("Compound_P"));
  PRINT_LINE(5);

  Serial.print('\n');
  Serial.print(F("Random"));
  PRINT_LINE(6);
  Serial.print(F("Random_P"));
  PRINT_LINE(7);

  Serial.print('\n');
  Serial.print(F("FilledRandom"));
  PRINT_LINE(8);
  
}



void setup() {
  Serial.begin(SERIAL_BAUD);
  display.println_P(0, PSTR("STARTING..."));
  delay(500);
  display.clear();

  testSpeedFull();
}

void loop() { }
