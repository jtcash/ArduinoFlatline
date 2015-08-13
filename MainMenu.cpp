/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*############################*/

/** A class to act as the LCD menu and update the LCD
 * according to the what line is selected. Will have submenus
 * which are interactive.
 */
#include "MainMenu.h"

#include "LoadMenu.h"
#include "SaveMenu.h"
#include "ResetMenu.h"
#include "CalibrateMenu.h"


MainMenu::MainMenu(){  
  idx = 0;
  subMenu = 0;

  // Set up the arrays
  lines[INDEX_JOBFEET]    = &MainMenu::showLineJobFeet;
  lines[INDEX_BELTSPEED]  = &MainMenu::showLineBeltSpeed;
  lines[INDEX_RESETJOB]   = &MainMenu::showLineResetJob;
  lines[INDEX_SAVEJOB]    = &MainMenu::showLineSaveJob;
  lines[INDEX_LOADJOB]    = &MainMenu::showLineLoadJob;
  lines[INDEX_CALIBRATE]  = &MainMenu::showLineCalibrate;
  lines[INDEX_TOTALWOOD]  = &MainMenu::showLineTotalWood;
  lines[INDEX_TOTALBELT]  = &MainMenu::showLineTotalBelt;
  lines[INDEX_HISTORY]    = &MainMenu::showLineHistory;

  subMenus[INDEX_JOBFEET]   = NULL;
  subMenus[INDEX_BELTSPEED] = NULL;
  subMenus[INDEX_RESETJOB]  = &resetMenu;
  subMenus[INDEX_SAVEJOB]   = &saveMenu;
  subMenus[INDEX_LOADJOB]   = &loadMenu;
  subMenus[INDEX_CALIBRATE] = &calibrateMenu;
  subMenus[INDEX_TOTALWOOD] = NULL;
  subMenus[INDEX_TOTALBELT] = NULL;
  subMenus[INDEX_HISTORY]   = NULL;

}

