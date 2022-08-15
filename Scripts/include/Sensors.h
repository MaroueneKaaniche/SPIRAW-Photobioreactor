#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// inclusion of needed libraries to interfaces with sesnors 

#include <OneWire.h>
#include <DallasTemperature.h>

// defining the sesnors pins (are random right now, still not properly defined)

#define FlowPin 20
#define pHPin 21
#define TempPin 22
#define TurbPin 23

//external variables 

extern portMUX_TYPE synch;
extern portMUX_TYPE timerMux;
extern int pulseCount;
extern unsigned long totalMilliLitres; 


// decleration of the functions used for data acquisition from sesnors

void SetupTempSensor();
float GetTemp(DallasTemperature sensors);
float GetpH();
float GetTurb();
void IRAM_ATTR GetFlow();
void IRAM_ATTR pulseCounter();
#endif