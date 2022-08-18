#include "Actuators.h"


// Turn On or off the desired actuator using the specified pin name

void TurnOn(int PinName){
        digitalWrite(PinName, HIGH);
}
void TurnOff(int PinName){
        digitalWrite(PinName, HIGH);
}

//controll motor with a given speed and direction.

void TurnMotor(int speed, int direction){
    if(direction==1){
        analogWrite(MotorA,speed);
    }
    else{
        analogWrite(MotorB,speed);
    }
}
// Stop motor rotation

void StopMotor(){
    digitalWrite(MotorA, LOW);
    digitalWrite(MotorB, LOW);
}

void SystemControl(void *parameter) {
  while(1) {
  }
}