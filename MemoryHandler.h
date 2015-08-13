
/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*############################*/

#ifndef MemoryHandler_h
#define MemoryHandler_h
#include <EEPROM.h>

#include "Arduino.h"
#include "MainMenu.h"
#include "LCDisplay.h"

#include "Job.h"

/* Memory locations */
#define MEM_START_OFFSET 0 // The starting offset in EEPROM for access

#define MEM_DEBUG_BYTES 40
#define MEM_RESET_HIST_BYTES 1
#define MEM_CALIBRATION_BYTES 4
#define MEM_PREV_SESSION_BYTES 4
#define MEM_TOTAL_LENGTH_BYTES 8

#define MEM_SAVE_SLOTS_BYTES 20
#define MEM_HIST_INDEX_BYTES 1
#define MEM_HIST_BYTES 400

#define MEM_DEBUG MEM_START_OFFSET
#define MEM_RESET_HIST MEM_DEBUG + MEM_DEBUG_BYTES
#define MEM_CALIBRATION MEM_RESET_HIST + MEM_RESET_HIST_BYTES
#define MEM_PREV_SESSION MEM_CALIBRATION + MEM_CALIBRATION_BYTES
#define MEM_TOTAL_LENGTH MEM_PREV_SESSION + MEM_PREV_SESSION_BYTES
#define MEM_SAVE_SLOTS MEM_TOTAL_LENGTH + MEM_TOTAL_LENGTH_BYTES

#define MEM_HIST_INDEX MEM_SAVE_SLOTS + MEM_SAVE_SLOTS_BYTES
#define MEM_HIST MEM_HIST_INDEX + MEM_HIST_INDEX_BYTES


#define DELAY_PRINT_INFO 750


// The total length vars
extern volatile unsigned long totalLength;
extern unsigned long totalBeltLength;


struct TotalLength{
  unsigned long mmWood;
  unsigned long mmBelt;
};

class MemoryHandler {
public:
  MemoryHandler() { };
  /// Save job to eeprom; not implemented fully yet. 
  void saveJob(byte slot){
    display.noBlink();
    
    display.println_P(0, PSTR("SAVING TO SLOT"));
    
    char slotStr[2];
    slotStr[0] = slot+1+'0';
    slotStr[1] = '\0';
    display.println(1,slotStr);

    // Save the totals to EEPROM
    saveTotal();
    
    unsigned long jobLength = job.getJobLength();
    EEPROM.put(MEM_SAVE_SLOTS+4*slot, jobLength);

    #if SERIAL_IS_ACTIVE
      Serial.print(F("Saved to slot "));
      Serial.print(slot+1);
      Serial.print(F("; Memory Address = "));
      Serial.print(MEM_SAVE_SLOTS+4*slot);
      Serial.print(F("; jobLength = "));
      Serial.println(jobLength);
    #endif
    delay(DELAY_PRINT_INFO);
  
    display.clear();
    display.println_P(0, PSTR("SAVED"));
    delay(DELAY_PRINT_INFO/2); //// TODO PROBABLY REPLACE ME
  
    if(ZERO_AFTER_SAVE)
      menu.idx = 0;
    
    
  }
  void loadJob(byte slot){
    display.noBlink();
    
    display.println_P(0, PSTR("LOADING SLOT"));
    
    char slotStr[2];
    slotStr[0] = slot+1+'0';
    slotStr[1] = '\0';
    display.println(1,slotStr);

    // Save the totals to EEPROM
    saveTotal();

    // Fetch the saved value from EEPROM
    unsigned long loadedLength;
    EEPROM.get(MEM_SAVE_SLOTS+4*slot, loadedLength);

    #if SERIAL_IS_ACTIVE
      Serial.print(F("Loaded from slot "));
      Serial.print(slot+1);
      Serial.print(F("; Memory Address = "));
      Serial.print(MEM_SAVE_SLOTS+4*slot);
      Serial.print(F("; jobLength = "));
      Serial.println(loadedLength);
    #endif

    // Load the job with the new value
    job.load(loadedLength);
    
    delay(DELAY_PRINT_INFO);
    
    display.clear();
    display.println_P(0, PSTR("DONE"));
    delay(DELAY_PRINT_INFO/2); //// TODO PROBABLY REPLACE ME
  
    if(ZERO_AFTER_SAVE)
      menu.idx = 0;
  }

  void saveTotal(){
    TotalLength tlStruct = { totalLength, totalBeltLength };
    EEPROM.put(MEM_TOTAL_LENGTH, tlStruct);
  }
  void loadTotal(){
    TotalLength tlStruct;
    EEPROM.get(MEM_TOTAL_LENGTH, tlStruct);
    totalLength = tlStruct.mmWood;
    totalBeltLength = tlStruct.mmBelt;
  }
  
};

extern MemoryHandler memoryHandler;


#endif
