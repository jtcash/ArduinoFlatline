/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*############################*/

#include <EEPROM.h>
#include <LiquidCrystal.h>

#include <TimerOne.h>

///////////// TODO //////////////
// Adapt all submenus to use fillLineBuff
///
// Make calibrate mode for job.h and call from CalibrateMenu
////COME UP WITH WAY TO HAVE A TEMPORARY FOOT STORAGE FOR THE CALIBRATION RECORDING AND USE IT TO TWEAK THE BELTCIRCUMFERENCE
// Have it start calibration mode when calibrating switches to true; save the current length to a variable
// Let recording turn true, and then when it turns false, calibration is done.
// Use the difference between jobLength and the length at the start of recording to find how long was recorded in calibration
// This length can be used to modify circumference using some algebra and the equation deltaX = circumference/rotationSpeed*deltaT
// Whatever deltaX it measures 

// Implement load and save methods in job.h
// 
////////////////////////////////// 


/////////////  NOTES ////////////// 
// Lengths are stored in mm
///////////////////////////////////

#include "LCDisplay.h"
#include "MainMenu.h"
#include "ResetMenu.h"
#include "LoadMenu.h"
#include "SaveMenu.h"
#include "CalibrateMenu.h"
#include "Buttons.h"
#include "MemoryHandler.h"


#define RATE_LCD_REFRESH 6      // Rate in HZ to refresh the display
#define RATE_BUTTON_CHECK 100  //Anything over 1000 will make a constant check.
#define RATE_SENSOR_CHECK 1000 //SHould be 1000ish


#define DELAY_CHECK_BUTTONS 1000/RATE_BUTTON_CHECK
#define DELAY_REFRESH_LCD 1000/RATE_LCD_REFRESH
#define DELAY_CHECK_SENSORS 1000/RATE_SENSOR_CHECK

#define SHOW_STARTUP_SCREEN false // Print to the screen to indicate startup

unsigned long woodInches = 0; // Temp name for num inches

unsigned long loopReps = 0; // The number of loop repetitions

unsigned long timeCurrent = 0; // Stores the current milliseconds since start

unsigned long timeRefreshLCD = 0; // The ms time to refresh the screen

unsigned long timeCheckButtons = 0; // The ms time to check the buttons

unsigned long timeCheckSensors = 0;

unsigned long tempA0 = 0; ///TODO MAKE THIS NOT TEMP

unsigned long beltAverageDiff = 0;


// The circumference of the wheel in mm, will be loaded from eeprom
unsigned long wheelCircumference = 330;


volatile unsigned long totalLength = 0;
unsigned long totalBeltLength = 0;

unsigned long totalBeltRotations = 0;

bool calibrating = false;

bool activeBoard = false;
bool activeWheel = false;

/** The LCDisplay object for writing to the LCD */
LCDisplay display(22, 23, 24, 25, 26, 27);

/** The various Menu Objects */
MainMenu menu;        
ResetMenu resetMenu;
LoadMenu loadMenu;
SaveMenu saveMenu;
CalibrateMenu calibrateMenu;

/** Object to handle reads and writes to the EEPROM */
MemoryHandler memoryHandler;

/** Object to handle checking for button presses */
Buttons buttons;

/** The Job Object which will check the sensors and handle calculations */
Job job;




/** The interrupt routine for when pin2, the Hall effect sensor, is triggered */
void pin2Interrupt(){
  if(digitalRead(2) == LOW){
    digitalWrite(PIN_BELT_LED, HIGH);
    job.beltInterrupt();
  } else {
    digitalWrite(PIN_BELT_LED, LOW);
  }
}

/** The interrupt routine for when the timer fires */ 
void timerOneInterrupt(){
  job.checkRecording();
}




void setup() {
  // Initialize the display for a 16x2 LCD
  display.begin(LCDISPLAY_WIDTH, LCDISPLAY_HEIGHT);
  
#if SHOW_STARTUP_SCREEN
  // Use display to print a starting message from PROGMEM
  display.println_P(0, PSTR("STARTING..."));
#endif
  
  // Set up Serial if it is activated
#if SERIAL_IS_ACTIVE // Found in Globals.h
  Serial.begin(SERIAL_BAUD);
#endif

  // Load the total lengths into memory
  memoryHandler.loadTotal();

  // Attach a timer interrupt for recording.
  //A uniform time between recordings is required for accuracy
  Timer1.initialize(DELAY_US_CHECK_RECORDING);
  Timer1.attachInterrupt(timerOneInterrupt);  

  // Attach an interrupt to pin2 to capture when the belt sensor fires.
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(PIN_2_INTERRUPT, pin2Interrupt, CHANGE);

#if SHOW_STARTUP_SCREEN
  // Delay the first refresh to keep the starting message long enough to see
  timeRefreshLCD = millis() + DELAY_REFRESH_LCD*2;
#endif

}




void loop() {
  // Increment the loop counter
  ++loopReps;

  // Get the current time in milliseconds.
  unsigned long timeCur = millis();
  // Using a local var here improves performance for some reason
  timeCurrent = timeCur; 
  

  // Check the sensors if it's time
  if (timeCur >= timeCheckSensors){
    job.checkSensors();
    timeCheckSensors = timeCur + DELAY_CHECK_SENSORS;
  }

  // Check buttons for presses if it's time
  if(timeCur >= timeCheckButtons){
    buttons.check();
    timeCheckButtons = timeCur + DELAY_CHECK_BUTTONS;
  }

  // Refresh the LCD if it's time
  if (timeCur >= timeRefreshLCD) {
    menu.updateMenu();    
    timeRefreshLCD = timeCur + DELAY_REFRESH_LCD;
  }

}
