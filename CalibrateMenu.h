/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*############################*/

#ifndef CalibrateMenu_h
#define CalibrateMenu_h

// LOTS OF WORK TO BE DONE IN THIS CLASS

#include "Arduino.h"
#include "LCDisplay.h"
#include "MainMenu.h"
#include "MemoryHandler.h"

#include "Job.h"

#include "SubMenu.h"

extern bool calibrating;

extern bool activeBoard;
extern bool activeWheel;


class CalibrateMenu : public SubMenu {
  public:
    byte feet;
    byte inches;


    CalibrateMenu() : feet(0), inches(0), selected(NULL)  { }

    void updateMenu() {
      //lcd.clear();
      //lcd.setCursor(0, 0);
      //lcd.home();
      //char buf[17];
      char lineBuffer[17];
      
      if(calibrating){
        
        snprintf_P(lineBuffer, sizeof(lineBuffer), PSTR("Calibrate %02d'%02d\""), feet, inches);
        display.println(0, lineBuffer);
        //lcd.setCursor(0, 1);
        //lcd.print("Insert");
        //char buf2[17];
        snprintf_P(lineBuffer, sizeof(lineBuffer), PSTR("board:%01d; wheel:%01d"), activeBoard, activeWheel);
        ///snprintf_P(buff, sizeof(buff), PSTR("%-16s"), buf2);
        //lcd.print(buff);
        display.println(1, lineBuffer);
      } else {
        snprintf_P(lineBuffer, sizeof(lineBuffer), PSTR("Length: %02d'%02d\"  "), feet, inches);
        display.println(0, lineBuffer);
        
        display.println_P(1, PSTR("Back   Calibrate"));
        byte x;
        byte y;
        switch (idx) {
          case 0:
            x = 0; y = 1;
            break;
          case 1:
            x = 9; y = 0;
            break;
          case 2:
            x = 12; y = 0;
            break;
          case 3:
            x = 7; y = 1;
            break;
          default:
            x = 0; y = 0;
            break;
        }
        display.setCursor(x, y);
        display.blink();
        if (selected != NULL)
          display.cursor();
        else
          display.noCursor();
          
      }
    }
    void selectItem() {
      // Pressing Select cancels calibration
      if(calibrating){
        calibrating = false;
        return;
      }
      switch (idx) {
        case 0:
          // Return to the main menu
          menu.subMenu = 0;
          selected = NULL;
          break;
        case 1:
          if (selected == NULL)
            selected = &feet;
          else
            selected = NULL;
          
          break;
        case 2:
          if (selected == NULL) 
            selected = &inches;
          else
            selected = NULL;
          
          break;
        case 3:
          calibrating = true;
          selected = NULL;
          break;
        default: // MIGHT NOT NEED DEFAULT
          menu.subMenu = 0;
          selected = NULL;
          break;
      }

    }
    void scrollUp() {
      if (selected == NULL) {
        if (idx != 0)
          --idx;
        else
          idx = CALIBRATE_CHOICES - 1;
          
      } else {
        --*selected;
        checkVars();
      }
    }
    void scrollDown() {
      if (selected == NULL) {
        if (idx < CALIBRATE_CHOICES - 1) // Not -1 because first slot is cancel save
          ++idx;
        else
          idx = 0;

      } else {
        ++*selected;
        checkVars();
      }
    }
  private:
    byte *selected;
    /// TODO REMOVE MAGIC NUMBERS
    /* Make sure the feet and inches have sane values, roll over */
    void checkVars() {
      if (inches == 12) {
        inches = 0;
        ++feet;
      } else if (inches == 255) {
        inches = 11;
        --feet;
      }
      if (feet == 50) {
        feet = 0;
      } else if (feet == 255) {
        feet = 0;
        inches = 0;
      }
    }
};

extern CalibrateMenu calibrateMenu;

#endif
