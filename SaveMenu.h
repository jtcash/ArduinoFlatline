/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*############################*/

#ifndef SaveMenu_h
#define SaveMenu_h

//#include <LiquidCrystal.h>
#include "LCDisplay.h"
#include "Arduino.h"
#include "MainMenu.h"
#include "MemoryHandler.h"


#include "SubMenu.h"


/** A submenu class to handle saving jobs */
class SaveMenu : public SubMenu{
public:
 
  SaveMenu() { }
  void updateMenu(){
    display.println_P(0, PSTR("Overwrite Slot:"));
    display.println_P(1, PSTR("Back  1 2 3 4 5"));
    if(idx==0)
      display.setCursor(0, 1);
    else 
      display.setCursor( 4+idx*2 , 1);
    
    display.blink();
  }
  void selectItem(){
    if(idx == 0){
      menu.subMenu = 0; // Return to the main menu
    } else {
      memoryHandler.saveJob(idx - 1);
      menu.subMenu = 0;
    }
  }
  void scrollUp(){
    if(idx != 0)
      --idx;
    else
      idx = SAVE_SLOTS;
  }
  void scrollDown(){
    if(idx < SAVE_SLOTS) // Not -1 because first slot is cancel save
      ++idx;
    else
      idx = 0;
  }
  
};

extern SaveMenu saveMenu;

#endif
