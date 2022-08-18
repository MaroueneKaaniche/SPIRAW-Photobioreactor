#include <Arduino.h>
#include "Actuators.h"
#include "Sensors.h"
#include "Wireless.h"
#include "Display.h"


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
float testTurb;

void setup() {
  Serial.begin(9600);
  
  //launching sensor instance
  sensors.begin();    
  //external interrupts on flowPin 
  attachInterrupt(FlowPin, pulseCounter, RISING);
  
  //timer interrupts every 1 second
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, GetFlow, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);    


  //Creation of diffrent tasks used in the system 
  
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              SystemControl,  // Function to be called
              "SystemControl",   // Name of task
              4096,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              1);     // Run on one core for demo purposes (ESP32 only)

  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              Display,  // Function to be called
              "SystemDisplay",   // Name of task
              2048,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              1);     // Run on one core for demo purposes (ESP32 only)

  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              WirlessCommunication,  // Function to be called
              "WirlessCommunication",   // Name of task
              2048,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              1);     // Run on one core for demo purposes (ESP32 only)


}

void loop() {
  testTurb=GetTurb();
  #ifdef DEBUG
    Serial.print("Turb is :");
    Serial.println(testTurb);
  #endif

}