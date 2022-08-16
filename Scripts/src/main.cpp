#include <Arduino.h>
#include "Actuators.h"
#include "Sensors.h"
#define DEBUG
// variables used to synchronize between main loop and the ISR when modifying a shared variable

portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// timer pointer 

hw_timer_t * timer = NULL; //

//setting up a oneWire instance to communicate the sensor

OneWire oneWire(TempPin);
DallasTemperature sensors(&oneWire);


//flowRate variables 

volatile int pulseCount;
unsigned long totalMilliLitres; 
float testTemp;

void setup() {
  Serial.begin(9600);
  
  //launching sensor instance
  sensors.begin();

  //external interrupts on flowPin 
 // attachInterrupt(FlowPin, pulseCounter, RISING);
  
  //timer interrupts every 1 second
  //timer = timerBegin(0, 80, true);
  //timerAttachInterrupt(timer, GetFlow, true);
  //timerAlarmWrite(timer, 1000000, true);
  //timerAlarmEnable(timer);    

}

void loop() {
  testTemp=GetTemp();
  #ifdef DEBUG
    Serial.print("temp is :");
    Serial.println(testTemp);
  #endif
  delay(1000);
}