#include "Display.h"
#include <Arduino.h>


void Display(void *parameter) {
    while(DisplaySerial.available()){
        DisplaySerial.readString();
    }
}
