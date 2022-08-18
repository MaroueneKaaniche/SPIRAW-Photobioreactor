#include <Arduino.h>
#include "Actuators.h"
#include "Sensors.h"
#include "Wireless.h"
#include "Display.h"

#define DebugSerial Serial // serial port used for DEBUG

#define DEBUG // comment to desable debug over DebugSerial port
// variables for timer instantiation
#define TIMER 0
#define DEVIDE 80
#define ALARM 1000000

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

//state machine variables 
enum states {                         // Define enumerated type for state machine states
  START, 
  INITIAL, 
  PRINCIPAL,
  TERMINAL 
};
states state;                         // Global variable to store current state

// Queue variables 
typedef struct msg {      
  char body[20];
  int count;
} msg;
static const int msg_queue_len = 5;     // Size of msg_queue
static QueueHandle_t msg_queue;         //handle of a queue 

// function for system control, to be called in the system control task 
void SystemControl(void *parameter) {
  switch (state) {
    case START:                     // upon system start, it awaits for the user to instruct the beginnig of cycle
        //do something
        break;  
    case INITIAL:                   
        //do something
        break;    
    case PRINCIPAL:
        //do something
        break;    
    case TERMINAL:           
        //do something
        break;    
  }
}


void setup() {
  #ifdef DEBUG
    DebugSerial.begin(9600);   //lfouuuk
  #endif
  //launching sensor instance
  sensors.begin();    
  //external interrupts on flowPin 
  attachInterrupt(FlowPin, pulseCounter, RISING);
  
  //timer interrupts every 1 second
  timer = timerBegin(TIMER, DEVIDE, true);  // define lfouk 
  timerAttachInterrupt(timer, GetFlow, true);
  timerAlarmWrite(timer, ALARM, true);      // kif kif
  timerAlarmEnable(timer);    
  

  state=START;    //When starting the ESP, the system is at start state and waiting to begin  

  msg_queue=xQueueCreate(msg_queue_len, sizeof(msg));  // creating a queue for handling msgs between tasks


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
    DebugSerial.print("Turb is :");
    DebugSerial.println(testTurb);
  #endif

}