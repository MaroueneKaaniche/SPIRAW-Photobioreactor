#ifndef AUCTUATORS_H
#define AUCTUATORS_H

#include <Arduino.h>

// defining the actuators pins (are random right now, still not properly defined)

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

// decleration of the functions used to manipulate actuators

void TurnOn(int PinName);
void TurnOff(int PinName);
void TurnMotor(int speed, int direction);
void StopMotor();


#endif