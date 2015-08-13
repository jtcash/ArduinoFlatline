/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*############################*/

/* Menu.h
 *  Header for the Main Menu class.
 */
#ifndef MainMenu_h
#define MainMenu_h


#include "Arduino.h"
#include "SubMenu.h"
#include "LCDisplay.h"

#include "Job.h" // for ImperialLength
#include "Globals.h"


#define NUM_ITEMS 9             // The number of items in the menu
#define RESET_CHOICES 2         // The number of choices in the reset menu
#define SAVE_SLOTS 5            // The number of save slots
#define CALIBRATE_CHOICES 4

#define ZERO_AFTER_SAVE true    // Return to index 0 after save


// The ordering of main menu items
#define INDEX_JOBFEET     0
#define INDEX_BELTSPEED   1
#define INDEX_RESETJOB    2
#define INDEX_SAVEJOB     3
#define INDEX_LOADJOB     4
#define INDEX_CALIBRATE   5
#define INDEX_TOTALWOOD   6
#define INDEX_TOTALBELT   7
#define INDEX_HISTORY     8


extern unsigned long loopReps;
extern unsigned long timeCurrent;
extern unsigned long tempA0;
extern unsigned long menuUpdateTime;
extern unsigned long beltAverageDiff;
extern unsigned long wheelCircumference;


extern volatile unsigned long totalLength;
extern unsigned long totalBeltLength;


/** A class to act as the LCD menu and update the LCD
 * according to the what line is selected. Will have submenus
 * which are interactive.
 */
class MainMenu{
public:
  byte idx;      //The current index
  byte subMenu;  // The current submenu being viewed
  
  /* An array of pointers to functions, each of which displays a line
   *  on the LCD. */
  void (MainMenu::*lines[NUM_ITEMS])(byte);
  SubMenu *subMenus[NUM_ITEMS];
  
  MainMenu();

  /* Functions to show each of the lines in the main menu */
  void showLineJobFeet(byte p){
    size_t bufferIdx = display.fillLineBuff_P(0, PSTR("0.Job "));
    
    bufferIdx = display.fillLineBuff(bufferIdx, (unsigned int)(loopReps/timeCurrent));
    bufferIdx = display.fillLineBuff(bufferIdx, ':');
    
    bufferIdx = display.fillLineBuff(bufferIdx, job.getImperialLength());
    display.terminateLineBuff(bufferIdx);
    display.printLineBuff(p);
  }
  
  void showLineBeltSpeed(byte p){
    size_t bufferIdx =  display.fillLineBuff_P(0, PSTR("1.ft/s: "));
    float fps = job.getBeltSpeed();
    int feet = (int)fps;
    bufferIdx =  display.fillLineBuff(bufferIdx, feet);
    
    size_t decimalIdx = bufferIdx;
    
    bufferIdx =  display.fillLineBuff(bufferIdx, (int)((fps-feet)*100)+100);
    display.fillLineBuff(decimalIdx, '.');
   
    display.terminateLineBuff(bufferIdx);
    display.printLineBuff(p);
  }
  void showLineResetJob(byte p){
    size_t bufferIdx =  display.fillLineBuff_P(0, PSTR("2.Reset ")); //PSTR("2:ResetJob"));
    bufferIdx = display.fillLineBuff(bufferIdx, tempA0); //beltAverageDiff);
    display.terminateLineBuff(bufferIdx);
    display.printLineBuff(p);
  }
  void showLineSaveJob(byte p){
    display.println_P(p, PSTR("3.SaveJob"));
  }
  
  void showLineLoadJob(byte p){
    display.println_P(p, PSTR("4.LoadJob"));
  }

  void showLineCalibrate(byte p){
    display.println_P(p, PSTR("5.Calibrate"));
  }
  
  void showLineTotalWood(byte p){
    size_t bufferIdx =  display.fillLineBuff_P(0, PSTR("6.Wood: "));
    unsigned long totalFeet = totalLength/304.8;
    bufferIdx =  display.fillLineBuff(bufferIdx, totalFeet);
  
    bufferIdx = display.fillLineBuff(bufferIdx, '\'');

    display.terminateLineBuff(bufferIdx);
    display.printLineBuff(p);
  }

  void showLineTotalBelt(byte p){
    display.println_P(p, PSTR("7.TotalBelt"));
  }

  // If this 
  void showLineHistory(byte p){
    display.println_P(p, PSTR("8.History"));
    
    // If there is a space after this item, show some more info on the next line
    if(p==0){
      size_t bufi = display.fillLineBuff_P(0, PSTR("Up: "));
      bufi = display.fillLineBuff(bufi, timeCurrent/1000);
      display.terminateLineBuff(bufi);

      display.printLineBuff(1);
    }
  }

  void selectItem(){
    // If the item is selectable
    if(!subMenu){
      SubMenu *sm = subMenus[idx];
      // Make sure the submenu exists and then select it
      if(sm != NULL){
        subMenu = idx;
        sm->idx = 0;
      }
    } else {
      // Select with the current submenu
      SubMenu *sm = subMenus[subMenu];
      if(sm != NULL)
        sm->selectItem();
    }
  }
  ///void selectItem();
  void scrollDown(){
    if(!subMenu){
      if(idx < NUM_ITEMS-1)
          ++idx;
          
    } else {
      // Scroll the current Sub Menu down
      SubMenu *sm = subMenus[subMenu];
      if(sm != NULL)
        sm->scrollDown();
    }
  }
  
  void scrollUp(){
    if(!subMenu){
      if(idx != 0)
        --idx;
        
    } else {
      // Scroll the current Sub Menu Up
      SubMenu *sm = subMenus[subMenu];
      if(sm != NULL)
        sm->scrollUp();
    }
  }
    
  // Update the screen with the main menu
  void updateMenu(){
    if(subMenu == 0){
      // Update the lcd with the menu items specified by idx

      // Get a pointer to a pointer to the current line printing function
      void (MainMenu::**line)(byte) = &lines[idx];
      // Call this function and if there's another after it, call that one too
      (this->**line)(0);
      if(idx < NUM_ITEMS-1)
        (this->**(line+1))(1);
        
      // If a line can open to a Sub Menu, highlight and blink
      if(subMenus[idx] != NULL){
        display.setCursor(0,0);
        ///display.home() // Do not use home() as this has a 2ms delay
        display.blink();
      } else 
        display.noBlink();
      
  
    } else {
      // THIS NULL CHECK CAN MAYBE BE REMOVED IF THE CODE IS ROBUST ENOUGH
      // Update the current Sub Menu
      SubMenu *sm = subMenus[subMenu];
      if(sm != NULL)
        sm->updateMenu();
      
    }
  
  }
  
};
extern MainMenu menu;

/*
// Here are the prototypes from before the functions got put in the header
// for possible performance benefits
  void selectItem();
  void scrollDown();
  void scrollUp();
  void showLineJobFeet(byte p);
  void showLineBeltSpeed(byte p);
  void showLineSaveJob(byte p);
  void showLineLoadJob(byte p);
  void showLineResetJob(byte p);
  void showLineTotalWood(byte p);
  void showLineTotalBelt(byte p);
  void showLineHistory(byte p);
  void showLineCalibrate(byte p);
  //void showLine8(byte p);
  void updateMenu();
*/

#endif
