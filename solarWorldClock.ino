/**********************************************************************
 SolarWorldClock
 Bart van der Aa
 info@bartvanderaa.com
 2013-2014
 
 General notes:
 The Solar World Clock (SWC) project has been in my mind for about 10 years now, and finally I found a suitable platform (the Arduino) to actually built it. 
 But, first things first, what is the WSC project about? And secondly, how can we built one?

 The first question is actually fairly easy to answer. Imagine the map of the world where the sun's intensity is projected on the surface of this map.   
 One could read the approximate time by tracing the location of the sun, just by identifying on which parts of the globe it is day- or nighttime. 
 This, in brief, is the working principle of the WSC. Obviously, it is difficult to know the time in absolute terms, but who needs that anyway ;-)
 
 Notes on alorithm:
 The program uses a real-time-clock (RTC) to keep track of date and time. Based on date, time, and location the solar intensity is computed.
 A vector of intensity values (0-255) is then send to ShiftPWM to control an array of LEDs. Time can now be read based on the digitally projected solar intensity. 
 
 Sunset-Sunrise part:
 Eastern longitude positive, Western longitude negative       
 Northern latitude positive, Southern latitude negative
 
 PWM part:
 If flickering of the LEDs occurs try to adjust the PWM frequency.
 
 Credits go to:
 Solar intensity part by The Renewable Resource Data Center (RReDC), downloaded 2014
 RTC part by Michael Margolis, downloaded 2014 
 PWM part by Elco Jacobs, updated August 2012
 
 ***********************************************************************/

// Initialize Shift PWM before loading libraries. Note that dataPin and clockPin are predefined on pin 11 and pin 13, respectively.
const int ShiftPWM_latchPin = 4;
const bool ShiftPWM_invertOutputs = false;
const bool ShiftPWM_balanceLoad = false;

// Include other libraries
#include <ShiftPWM.h>
#include <Wire.h>
#include "RTClib.h"  
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "solpos00.h" 

// Initialize RTC
RTC_DS1307 RTC;

// Initialize shift-registers
void inOutAll(void);
unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;
unsigned int numRegisters = 3;
unsigned int numOutputs = numRegisters*8;

// Coordinate settings
double lat = 00.70;// 57.70; // lattitude, Göteborg
double lonVec[] = {
  -165, -150, -135, -120, -105, -90, -75, -60, -45, -30, -15, 0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180}; // Time-zone central longitude coordiantes 
double intensityVec[24];
//int normIntensity[24];
double maxIntensity = 1375; 

int *normIntensity = (int*)calloc(24,sizeof(int));

/*============================================================================*/
// Setup loop
/*============================================================================*/
void setup(){

  // Begin the Serial connection 
  Serial.begin(9600);

  // Instantiate the RTC
  Wire.begin();
  RTC.begin();

  // Check if the RTC is running.
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running");
  }

  // This section grabs the current datetime and compares it to
  // the compilation time.  If necessary, the RTC is updated.
  DateTime now = RTC.now();
  DateTime compiled = DateTime(__DATE__, __TIME__);
  if (now.unixtime() < compiled.unixtime()) {
    Serial.println("RTC is older than compile time! Updating");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  // Sets the number of 8-bit registers that are used.
  ShiftPWM.SetAmountOfRegisters(numRegisters);
  
  // Set PWM frequency and Maximum brightness
  ShiftPWM.Start(pwmFrequency,maxBrightness);
  
  Serial.println("Setup complete.");
}

/*============================================================================*/
// main loop 
/*============================================================================*/
void loop(){    

  // Solar intensity alg initi
  struct posdata pd, *pdat;
  long retval;
  pdat = &pd;
  S_init(pdat);

  // Get actual time and reformat
  DateTime now = RTC.now();   
  pdat->function &= ~S_DOY;
  pdat->month = now.month();
  pdat->day   = now.day();
  pdat->year  = now.year(); 
  pdat->hour  = now.hour();
  pdat->minute = now.minute();
  pdat->second = now.second();
  pdat->timezone  = 1.0;

  // Solar intensity
  pdat->latitude  =  lat; 
  for (int ii=0; ii<24; ii++){  
    pdat->longitude = lonVec[ii];
    retval = S_solpos(pdat);
    intensityVec [ii] = pdat->etr;
    normIntensity[ii] = round((pdat->etr)/maxIntensity*255);
  }  

  if(Serial.available()){
    if(Serial.peek() == 'l'){
      // Print information about the interrupt frequency, duration and load on your program
      ShiftPWM.PrintInterruptLoad();
    }
    while (Serial.read() >= 0){
      ; // flush remaining characters
    }
  }
  
  // Switch all Pins on
  ShiftPWM.Set24Pins(normIntensity);

  
  // Display the current time
  Serial.print("Current time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  
  Serial.println();
  for (int ii=0; ii<24; ii++){
    Serial.print(normIntensity[ii]);
    Serial.println();
  } 
  
  Serial.println();
  delay(5000);
}
