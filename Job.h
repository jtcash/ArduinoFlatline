/*############################*\
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this code is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
\*############################*/

#ifndef Job_h
#define Job_h

#include "Job.h"
#include "Arduino.h"
#include "MainMenu.h"
#include "LCDisplay.h"

#include "Globals.h"


#define DELAY_US_CHECK_RECORDING 2000 // The microsecond delay to check sensors
// 2000 us is plenty of time to run the recording interrupt, as it 
//runs in less than 60us in the best case and less than 250us in the worst case

#define MAX_BELT_TIME_DIFF 20000 // The max time for one revolution of the belt


#define RECORDING_LED_PIN 5 // The pin to light an LED when isRecording == true

#define NUM_IR_PINS 2  // The number of analog ins for the IR sensor
#define PIN_IR_SENSOR A1
#define PIN_IR_SENSOR_2 A0



#define SMOOTHING_X 8     // The smoothing factor for smootherX
#define SMOOTHING_V 4     // The smoothing factor for smootherV
#define SMOOTHING_BELT 2  // The smoothing factor for smootherBelt

#define TRIGGER_THRESHOLD 10 // The smoothed sensor velocity threshold at which to start recording

extern unsigned long timeCurrent;
extern unsigned long wheelCircumference;
extern volatile unsigned long totalLength;
extern unsigned long totalBeltLength;
extern unsigned long totalBeltRotations;

class Job {
public:
  
  Job() : smootherX(SMOOTHING_X), smootherV(SMOOTHING_V), smootherBelt(SMOOTHING_BELT){
    measurements = 0;
    
    timeCur = 0;
    timeBeltTriggered = 0;
    jobLength = 0;

    distPool = 0;

    prevBeltDiff = 0;

    beltTriggered = false;
    interruptRunning = false;
    isRecording = false;
    isBeltStopped = true;
    
    pinMode(RECORDING_LED_PIN, OUTPUT);
  }

  /** Called from a timed interrupt */
  void checkRecording(){
    // TEMP TO MAKE SURE IT DOESN'T INTERRUPT ITSELF
    if(interruptRunning){
      Serial.println(F("CONCURRENT CHECKRECORDING INTERRUPTS"));
      return;
    } else {
      interruptRunning = true;
    }
    // Re-enable interrupts so the belt can be triggered
    interrupts();
    if(isRecording){
      checkStopRecording();
      record();
    } else {
      checkStartRecording(); 
    }
    interruptRunning = false;
    noInterrupts();
  }

  void checkSensors(){
    timeCur = timeCurrent;

    if(timeCur - timeBeltTriggered > prevBeltDiff*2)
      isBeltStopped = true;
    
    // If the belt has been triggered, call beltUpdate()
    if(beltTriggered)
      beltUpdate();

    // Read values from the IR rangefinder and update smootherX, smootherV, diffSensor.
    readDistanceSensor();

    ++measurements;
  }


  /** Record the current piece of wood in the sensor. Calculate
   * how much time wood has traveled since last call of record */
  void record(){
    // Make sure the belt trigger times are recent enough
    if(isBeltStopped)
      return;
    unsigned long beltDiff = smootherBelt.getValue();
    // Make sure beltDiff has a sane value
    if(beltDiff == 0)
      return;
      
    float deltaXf = (float)wheelCircumference/(float)beltDiff*\
        (float)(DELAY_US_CHECK_RECORDING/1000.0) + distPool;
        
    unsigned long deltaX = (unsigned long)deltaXf;
    distPool = deltaXf - deltaX;
    jobLength += deltaX;
    totalLength += deltaX;
  }


  /** Check if a board has entered the beam, should start recording */
  inline void checkStartRecording(){
    if(smootherV.getValue() >= TRIGGER_THRESHOLD)
      startRecording();
  }
  /** Check if a board has exited the beam, should start recording */
  inline void checkStopRecording(){
    if(smootherV.getValue() <= -TRIGGER_THRESHOLD)
      stopRecording();
  }

  /** Start and stop recording */
  inline void startRecording(){
    isRecording = true;
    digitalWrite(RECORDING_LED_PIN, HIGH);
  }
  inline void stopRecording(){
    isRecording = false;
    digitalWrite(RECORDING_LED_PIN, LOW);
  }

  /* Interrupt function for when the belt sensor is triggered */
  inline void beltInterrupt(){
    beltTriggered = true;
  }

  /* Handle the the beltTrigger firing */
  void beltUpdate(){
    beltTriggered = false;
    
    timeBeltTriggered = timeCur;
    diffBelt.insert(timeCur);
    
    unsigned long beltDiff = diffBelt.getV();
    prevBeltDiff = (unsigned int)beltDiff;
    smootherBelt.insert((int)beltDiff);
    
    isBeltStopped = false;
    
    ++totalBeltRotations;
  }

  /* Read values from the distance sensor using two analog in pins for added accuracy.
   * Update the smoothers and differential.
   */
  void readDistanceSensor(){
    // Grab values from two sensors to reduce error
    int sensorVal = analogRead(PIN_IR_SENSOR) + analogRead(PIN_IR_SENSOR_2);

    // Fill smoothers and diffs to calm the signal and get a smooth derivative
    smootherX.insert(sensorVal);
    diffSensor.insert(smootherX.getValue());
    smootherV.insert(diffSensor.getV());
  }
  
  
// Externally called functions
  /* Reset the job */
  void reset(){
    // TODO Reimplement this method
  }

  /* Load a job */
  void load(unsigned long loadValue){
    // TODO Reimplement this method
  }

  
  float getBeltSpeed(){
    if(isBeltStopped)
      return 0.0;

    // If beltDiff is 0, will divide by 0 unless caught
    unsigned long beltDiff = smootherBelt.getValue();
    if(beltDiff == 0)
      return 0.0;
    
    return wheelCircumference*1000.0/beltDiff/304.8;
  }
  
  unsigned long getJobLength(){
    return jobLength;
  }
  
  ImperialLength getImperialLength(){
    unsigned long jobInches = (jobLength*10)/254;
    ImperialLength impLen = {jobInches/12,(byte)(jobInches%12)};
    return impLen;
  }
  
private:

  unsigned long measurements; // The number of measurements taken
  
  unsigned long timeCur; // The current time, held locally
  unsigned long timeBeltTriggered; // The last time the belt was triggered
  
  volatile unsigned long jobLength; // The length of the current job in mm
  
  // A pool of partial millimeters to be collected and added to jobLength
  volatile float distPool; 
  
  // The previous difference in time between belt interrupts
  volatile unsigned int prevBeltDiff; 

  volatile bool beltTriggered; // True if the belt has been triggered
  volatile bool interruptRunning; // TEMP value to ensure no concurrent interrupts
  volatile bool isRecording; // True if currently is recording length
  volatile bool isBeltStopped; // True if belt has stopped

  Differential diffSensor; // Used to find the derivative of sensor values
  TimeDifferential diffBelt; // Used to find the derivative of belt interrupt times

  Smoother smootherX; // Used to smooth the sensor readings to reduce noise
  Smoother smootherV; // Used to smooth the derivative of the sensor readings
  
  Smoother smootherBelt; // Used to smooth the belt readings to reduce error
  
};
extern Job job;


#endif
