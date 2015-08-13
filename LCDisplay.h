/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*############################*/

#ifndef LCDisplay_h
#define LCDisplay_h

#include <LiquidCrystal.h>
#include "Arduino.h"
#include "Globals.h"


#define LCDISPLAY_WIDTH 16
#define LCDISPLAY_HEIGHT 2
#define CHAR_ARRAY_CLEAR '\0'


/** This entire class is written in a header file because this allows GCC to
 * better optimize the code with methods such as inlining and whatnot.
 */
class LCDisplay {
public:
  /** Initialize a LCDisplay for a 4bit mode LiquidCrystal display. Takes the
   * same parameters as the LiquidCrystal constructor for 4 bit mode.
   *  This class is basically a performance-enhancing wrapper for LiquidCrystal,
   * but has some limitations in functionality for the sake of greatly improved
   * performance. For example, this class is intended for use with a 16x2
   * display, and would require modifications to work with greater dimensions.
   */
  LCDisplay(uint8_t rs,  uint8_t enable, uint8_t d0, 
      uint8_t d1, uint8_t d2, uint8_t d3) :
      lcd(rs, enable, d0, d1, d2, d3), isCursor(false), isBlink(false),
      cursorX(0), cursorY(0) {
        
    // Call begin for the display's dimensions
    lcd.begin(LCDISPLAY_WIDTH, LCDISPLAY_HEIGHT);

    // Clear the line arrays, which act as virtual lines for the LCD
    clearLine0();
    clearLine1();
    
  }

  /** Functions to clear the line arrays either from index 0, or from a
   * specified starting index to the end */
  inline void clearLine0(){ memset(line0, CHAR_ARRAY_CLEAR, sizeof(line0)); }
  inline void clearLine0(uint8_t startIdx){
    if(startIdx < sizeof(line0))
      memset(line0 + startIdx, CHAR_ARRAY_CLEAR, sizeof(line0)-startIdx);
  }
  inline void clearLine1(){ memset(line1, CHAR_ARRAY_CLEAR, sizeof(line1)); } // TODO Replace sizeofs with LCDISPLAY_WIDTH for consistency?
  inline void clearLine1(uint8_t startIdx){
    if(startIdx < sizeof(line1))
      memset(line1 + startIdx, CHAR_ARRAY_CLEAR, sizeof(line1)-startIdx);
  }
  inline void clearLine(uint8_t lineNum, uint8_t startIdx){
    switch(lineNum){
      case 0:   clearLine0(startIdx); break;
      case 1:   clearLine1(startIdx); break;
    }
  }

  /** Clears a specified number of bytes at the location specified by
   * cursorX and cursorY from the line array */
  inline void clearPrintedRange(uint8_t printedBytes){
    // Prevent overflows.
    if(cursorX + printedBytes > LCDISPLAY_WIDTH)
      printedBytes = LCDISPLAY_WIDTH - cursorX;
    // Clear the number of printed bytes from the line array
    switch (cursorY){
      case 0:
        memset(line0 + cursorX, CHAR_ARRAY_CLEAR, printedBytes);
        break;
      case 1:
        memset(line1 + cursorX, CHAR_ARRAY_CLEAR, printedBytes);
        break;
    }
    // Move the cursor according to number of bytes cleared
    cursorX += printedBytes;
  }
  
  /* Default lcd operations for print. Their use is strongly discouraged */
  template<typename D>
  size_t print(D data){
    // Print to the display
    size_t printedBytes = lcd.print(data);
    // Clear the line array where the data was printed
    clearPrintedRange(printedBytes);
    return printedBytes;
  }
  template<typename D, typename B>
  size_t print(D data, B base){
    // Print to the display
    size_t printedBytes = lcd.print(data, base);
    // Clear the line array where the data was printed
    clearPrintedRange(printedBytes);
    return printedBytes;
  }

  inline void begin(uint8_t x, uint8_t y){
    if(x == LCDISPLAY_WIDTH && y == LCDISPLAY_HEIGHT)
      lcd.begin(LCDISPLAY_WIDTH, LCDISPLAY_HEIGHT);
  }
  
  void clear(){
    cursorX = 0;
    cursorY = 0;
    isBlink = false;
    isCursor = false;
    // Wipe line arrays with spaces (' ')
    memset(line0, ' ', sizeof(line0));
    memset(line1, ' ', sizeof(line1));
    lcd.clear();
  }
  // Causes 2ms delay. Use setCursor(0,0) instead
  inline void home(){
    if(cursorX != 0 || cursorY != 0){
      cursorX = 0;
      cursorY = 0;
      lcd.home();
    }
  }
  
  inline void setCursor(uint8_t x, uint8_t y){
    if(cursorX != x || cursorY != y){
      cursorX = x;
      cursorY = y;
      lcd.setCursor(x,y);
    }
  }
  
  inline void blink(){
    if(!isBlink){
      isBlink = true;
      lcd.blink();
    }
  }
  inline void noBlink(){
    if(isBlink){
      isBlink = false;
      lcd.noBlink();
    }
  }
  
  inline void cursor(){
    if(!isCursor){
      isCursor = true;
      lcd.cursor();
    }
  }
  
  inline void noCursor(){
    if(isCursor){
      isCursor = false;
      lcd.noCursor();
    }
  }

  /*  Uses the Print::write method in the LCD object to write a single byte
   * and increments cursorX */
  inline size_t write(char data){
    return write((uint8_t) data);
  }
  inline size_t write(uint8_t data){
    ++cursorX;
    return lcd.write(data);
  }

  
/* Print text from progmem string str to line specified by line; 0 or 1*/
  void println(uint8_t lineNum, const char *str){
    // A pointer to the current line array to write with
    char *line;
    switch(lineNum){
      case 0:   line = line0; break;
      case 1:   line = line1; break;
      default:  return;
    }
    int i;
    // Iterate through the str char by char
    for(i=0; i<LCDISPLAY_WIDTH; ++i){
      char c = str[i];
      // Break if the string is terminated with the current char
      if(c=='\0')
        break;
      // If the char is not already on the screen, write it to the screen
      //and update the line array
      if(c != line[i]){
        line[i] = c;
        setCursor(i,lineNum);
        write(c);
      }
    }
    // Write spaces to the remainder of the line
    for(;i<LCDISPLAY_WIDTH;++i)
      if(line[i] != ' '){
        line[i] = ' ';
        setCursor(i,lineNum);
        write(' ');
      }
  }
  void println_P(uint8_t lineNum, const char *str){
    // A pointer to the current line array to write with
    char *line;
    switch(lineNum){
      case 0:   line = line0; break;
      case 1:   line = line1; break;
      default:  return;
    }                
      
    int i;
    // Iterate through the str in PROGMEM by reading it byte by byte.
    for(i=0; i<LCDISPLAY_WIDTH; ++i){
      // Get the current char in the str from PROGMEM
      char c = pgm_read_byte_near(str + i);
      // Break if the string is terminated with the current char
      if(c=='\0')
        break; 
      // If the char is not already on the screen, write it to the screen
      //and update the line array
      if(c != line[i]){
        line[i] = c;
        setCursor(i,lineNum);
        write(c);
      }
    }
    // Fill the remainder of the line with spaces
    for(;i<LCDISPLAY_WIDTH;++i)
      if(line[i] != ' '){
        line[i] = ' ';
        setCursor(i,lineNum);
        write(' ');
      }
  }

  /* Print a line from the line buffer */
  void printLineBuff(uint8_t lineNum){
    println(lineNum, lineBuff);
  }

  /** The following are functions to fill the line buffer prior to printing */
  inline void terminateLineBuff(size_t i){
    lineBuff[i] = '\0';
  }
  
  /** Loads into the line buffer, starting at index i, a string from PROGMEM
   *  Returns the index after the last written to.
   */
  size_t fillLineBuff_P(size_t i, const char *src){
    // Get a pointer to the starting place in lineBuff
    char *dest = lineBuff+i;
    for(; i<LCDISPLAY_WIDTH; ++i){
      // Read a char of src from PROGMEM
      char c = pgm_read_byte_near(src);
      // If src reaches terminator, return i;
      if(c == '\0')
        return i;
      // Update lineBuff and step dest forward
      *dest++ = c;
      src++;
    }
    return i;
  }
  size_t fillLineBuff_P(const char *src){
    return fillLineBuff_P(0, src);
  }
  
   /** Loads into the line buffer, starting at index i, a string from RAM
   *  Returns the index after the last index written to.
   */
  size_t fillLineBuff(size_t i, const char *src){
    // Get a pointer in lineBuff of where to start writing
    char *dest = lineBuff+i;
    for(; i<LCDISPLAY_WIDTH; ++i){
       char c = *src;
       if(c == '\0')
         return i;
       *dest++ = c;
       src++;
    }
    return i;
  }
  size_t fillLineBuff(const char *src){
    return fillLineBuff(0, src);
  }

  /** Place a char into the lineBuff and then return i+1 */
  inline size_t fillLineBuff(size_t i, char c){
    lineBuff[i] = c;
    return i+1;
  }
  /** Print ints, uints, longs and ulongs into the lineBuffer and
    * return the index after the last written char. Uses snprintf
    * for the integer to char array translation */
  inline size_t fillLineBuff(size_t i, int num){
    i += snprintf_P(lineBuff+i, sizeof(lineBuff)-i, PSTR("%d"), num);
    if(i > sizeof(lineBuff))
      i = sizeof(lineBuff) - 1;
    return i;
  }
  inline size_t fillLineBuff(size_t i, unsigned int num){
    i += snprintf_P(lineBuff+i, sizeof(lineBuff)-i, PSTR("%u"), num);
    if(i > sizeof(lineBuff))
      i = sizeof(lineBuff) - 1;
    return i;
  }
  inline size_t fillLineBuff(size_t i, long num){
    i += snprintf_P(lineBuff+i, sizeof(lineBuff)-i, PSTR("%ld"), num);
    if(i > sizeof(lineBuff))
      i = sizeof(lineBuff) - 1;
    return i;
  }
  inline size_t fillLineBuff(size_t i, unsigned long num){
    i += snprintf_P(lineBuff+i, sizeof(lineBuff)-i, PSTR("%lu"), num);
    if(i > sizeof(lineBuff))
      i = sizeof(lineBuff) - 1;
    return i;
  }
  inline size_t fillLineBuff(size_t i, ImperialLength impLen){
    i += snprintf_P(lineBuff+i, sizeof(lineBuff)-i, PSTR("%lu'%d\""), impLen.feet, impLen.inches);
    if(i > sizeof(lineBuff))
      i = sizeof(lineBuff) - 1;
    return i;
  }
  

#if SERIAL_IS_ACTIVE
// Print out the line buffer graphically for debugging
  void debugLineBuff(){
    int i;
    Serial.print("\n|  ");
    for(i=0; i<LCDISPLAY_WIDTH+1; ++i){
      Serial.print(i%10);
      Serial.print(F("|  "));
    }
    Serial.print("\n|");
    int c;
    for(i=0; i<LCDISPLAY_WIDTH+1; ++i){
      c = lineBuff[i];
      if(c<100){
        Serial.print(' ');
        if(c<10)
          Serial.print(' ');
      }
      Serial.print(c);
      //Serial.print((int)lineBuff[i]);
      Serial.print('|');
    }
    Serial.print("\n|");
    for(i=0; i<LCDISPLAY_WIDTH+1; ++i){
      Serial.print(' ');
      if(lineBuff[i] == 0)
       Serial.print(' ');
      Serial.print(lineBuff[i]);
      Serial.print(' ');
      Serial.print('|');
    }
    Serial.println("\n");
  }
#endif


  inline char *getLineBuff(){
    return lineBuff;
  }
private:
  LiquidCrystal lcd;

  bool isCursor;
  bool isBlink;
  uint8_t cursorX;
  uint8_t cursorY;

  char line0[LCDISPLAY_WIDTH];
  char line1[LCDISPLAY_WIDTH];
  
  char lineBuff[LCDISPLAY_WIDTH+1];
};

extern LCDisplay display;

#endif
