/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*############################*/

#ifndef ResetMenu_h
#define ResetMenu_h

#include "Arduino.h"
#include "MainMenu.h"
#include "Job.h"
#include "LCDisplay.h"
#include "MemoryHandler.h"

#include "SubMenu.h"

class ResetMenu : public SubMenu{
public:

  ResetMenu() { }
  void updateMenu(){
    display.println_P(0, PSTR("Reset Current?"));
    display.println_P(1, PSTR("  Back   RESET"));
    if(idx==0)
      display.setCursor(2, 1);
    else
      display.setCursor(9, 1);
    
    display.blink();
  }
  void selectItem(){
    if(idx == 0){
      menu.subMenu = 0; // Return to the main menu
    } else {
      display.noBlink();
      resetJob();
      menu.subMenu = 0;
    }
  }
  void scrollUp(){
    if(idx != 0)
      --idx;
    else
      idx = RESET_CHOICES;
  } 
  //TODO Optimize these two functions if there will only be 2 choices;
  void scrollDown(){
    if(idx < RESET_CHOICES - 1) 
      ++idx;
    else
      idx = 0;
  }


  void resetJob(){
    display.clear();
    //display.setCursor(0, 0);
    display.println_P(0, PSTR("RESETTING JOB"));
    job.reset();
    
    // Save the totals to EEPROM
    memoryHandler.saveTotal();
    
    delay(500); // Delay to show on screen
    
    display.println_P(0, PSTR("DONE"));
    delay(500); /// Delay to show on screen
    
    if (ZERO_AFTER_SAVE)
      menu.idx = 0;
      
  }

};

extern ResetMenu resetMenu;

#endif
