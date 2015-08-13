/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*############################*/

#ifndef SubMenu_h
#define SubMenu_h
#include "Arduino.h"

class SubMenu {
public:
  byte idx;
  SubMenu() : idx(0) { }
  virtual void updateMenu() = 0;
  virtual void selectItem() = 0;
  virtual void scrollUp() = 0;
  virtual void scrollDown() = 0;
};
#endif
