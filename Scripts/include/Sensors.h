#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <driver/gpio.h>
// inclusion of needed libraries to interfaces with sesnors 

#include <OneWire.h>
#include <DallasTemperature.h>

// defining the sesnors pins (are random right now, still not properly defined)

#define FlowPin 20
#define pHPin 22
#define TempPin GPIO_NUM_12
#define TurbPin GPIO_NUM_15

//external variables 

extern portMUX_TYPE synch;
extern portMUX_TYPE timerMux;
extern volatile int pulseCount;
extern unsigned long totalMilliLitres; 

extern DallasTemperature sensors;



// decleration of the functions used for data acquisition from sesnors

void SetupTempSensor();
float GetTemp();
float GetpH();
float GetTurb();
void IRAM_ATTR GetFlow();
void IRAM_ATTR pulseCounter();
#endif