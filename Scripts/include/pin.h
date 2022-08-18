#ifndef PIN_H
#define PIN_H

#include <Arduino.h>
#include <driver/gpio.h>

// actuators pins
#define AirPin 1
#define Water1Pin 2
#define Water2Pin 3
#define MicroPumpPin 4
#define Vanne1 5
#define Vanne2 6
#define Vanne3 7
#define Vanne4 8
#define Vanne5 9
#define MotorA 10
#define MotorB 11

//Display pins
#define RX GPIO_NUM_16
#define TX GPIO_NUM_17
#define DisplaySerial Serial2

//Sensors pins
#define FlowPin 20
#define pHPin 22
#define TempPin GPIO_NUM_12
#define TurbPin GPIO_NUM_15

#endif