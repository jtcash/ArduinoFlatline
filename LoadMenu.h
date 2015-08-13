/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*############################*/

#ifndef LoadMenu_h
#define LoadMenu_h

#include "LCDisplay.h"
#include "Arduino.h"
#include "MainMenu.h"
#include "MemoryHandler.h"

#include "SubMenu.h"

/** A submenu class to handle loading jobs */
class LoadMenu : public SubMenu{
public:

  LoadMenu() { }
  void updateMenu(){
    display.println_P(0, PSTR("Load Slot:"));
    display.println_P(1, PSTR("Back  1 2 3 4 5"));
    // Hover the cursor over the selected position
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
      memoryHandler.loadJob(idx - 1);
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

extern LoadMenu loadMenu;

#endif
