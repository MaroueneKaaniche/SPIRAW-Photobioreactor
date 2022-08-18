#ifndef AUCTUATORS_H
#define AUCTUATORS_H

#include <Arduino.h>
#include "pin.h"


// decleration of the functions used to manipulate actuators

void TurnOn(int PinName);
void TurnOff(int PinName);
void TurnMotor(int speed, int direction);
void StopMotor();
#endif