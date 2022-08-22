#include <Arduino.h>
#include "Actuators.h"
#include "Sensors.h"
#include "Wireless.h"
#include "Display.h"
#include "pin.h"

#define DebugSerial Serial // serial port used for DEBUG

#define DEBUG // comment to desable debug over DebugSerial port
// variables for timer instantiation
#define TIMER 0
#define DEVIDE 80
#define ALARM 1000000
#define Baudrate 9600
// variables used to synchronize between main loop and the ISR when modifying a shared variable
portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// timer pointer 
hw_timer_t * timer = NULL; //

//setting up a oneWire instance to communicate the sensor
OneWire oneWire(TempPin);
DallasTemperature sensors(&oneWire);


//sensor variables 
volatile int pulseCount;
unsigned long totalMilliLitres; 
unsigned long currentTime=millis();
int TimeForLights=12000;
int TimeForCleanup=12000;
float Temperature,pH,Turbidity,Flow;
float Xvalue;      //Turbidity threshhold
float Yvalue;      //Flowrate threshhold      
boolean cuurentState;

//state machine variables 
enum states {                         // Define enumerated type for state machine states
  START, 
  INITIAL, 
  PRINCIPAL,
  TERMINAL,
  START2 
};
states state;                         // Global variable to store current state

// Queue variables 
typedef struct message {      
    float Temperature,pH,Turbidity;
    unsigned long Flow;
} message;
static const int msg_queue_len = 5;     // Size of msg_queue
static QueueHandle_t msg_queue;         //handle of a queue 

// mutex
static SemaphoreHandle_t advanceToNextState;

// function for system control, to be called in the system control task 
void SystemControl(void *parameter) {
  message msg;
  msg.Flow=0;
  while(1){  
    switch (state) {
      case START:                     // upon system start, it awaits for the user to instruct the beginnig of cycle
        break;
      case INITIAL:                   
          TurnOn(MicroPumpPin);
          TurnOn(UVLightPin);
          vTaskDelay(20000);
          TurnOff(MicroPumpPin);
          TurnOff(UVLightPin);
          xQueueSend(msg_queue,(void *)&msg,10);
          xSemaphoreTake(advanceToNextState, portMAX_DELAY);
          state=PRINCIPAL;
          break;    
      case PRINCIPAL:
          TurnOn(AirPin);
          TurnOn(LEDLightPin);
          cuurentState=1;
          while(msg.Turbidity!=Xvalue){
            if((millis()-currentTime)>TimeForLights){
              if(cuurentState){
                  TurnOff(LEDLightPin);
                  cuurentState=0;
                  }
              else{
                TurnOn(LEDLightPin);
                cuurentState=1;
                }
            }
            msg.Temperature=GetTemp();
            msg.pH=GetpH();
            msg.Turbidity=GetTurb();
            xQueueSend(msg_queue, (void *)&msg , 10);
          }
          TurnOff(LEDLightPin);
          TurnOff(AirPin);
          xSemaphoreTake(advanceToNextState, portMAX_DELAY);
          TurnOn(Vanne1);
          TurnOn(Vanne2);
          TurnOn(Vanne3);
          TurnOn(Water1Pin);
          TurnOn(Water2Pin);
          TurnMotor(255,1);
          while(msg.Flow!=Yvalue){
            msg.Temperature=GetTemp();
            msg.pH=GetpH();
            msg.Turbidity=GetTurb();
            msg.Flow=totalMilliLitres;
            xQueueSend(msg_queue, (void *)&msg , 10);
          }
          TurnOff(Vanne1);
          TurnOff(Vanne2);
          TurnOff(Vanne3);
          TurnOff(Water1Pin);
          TurnOn(Water2Pin); 
          StopMotor();
          state=TERMINAL;       
          break;    
      case TERMINAL:           
          TurnOn(Vanne1);
          TurnOn(Vanne2);
          TurnOn(Vanne3);
          TurnOn(Water1Pin);
          TurnOn(Water2Pin);
          TurnMotor(255,1);
          vTaskDelay(TimeForCleanup);
          TurnOff(Vanne1);
          TurnOff(Vanne2);
          TurnOff(Vanne3);
          TurnOff(Water1Pin);
          TurnOn(Water2Pin); 
          StopMotor();
          state=START2;                      
          break;
      case START2:
          TurnOn(MicroPumpPin);
          vTaskDelay(20000);
          TurnOff(MicroPumpPin);
          xSemaphoreTake(advanceToNextState, portMAX_DELAY);
          state=PRINCIPAL;
          break;    
      }
  }
}

void setup() {
  #ifdef DEBUG
    DebugSerial.begin(Baudrate);   
  #endif
  //launching sensor instance
  sensors.begin();    
  //external interrupts on flowPin 
  attachInterrupt(FlowPin, pulseCounter, RISING);
  
  //timer interrupts every 1 second
  timer = timerBegin(TIMER, DEVIDE, true);  
  timerAttachInterrupt(timer, GetFlow, true);
  timerAlarmWrite(timer, ALARM, true);      
  timerAlarmEnable(timer);    
  

  state=START;    //When starting the ESP, the system is at start state and waiting to begin  

  msg_queue=xQueueCreate(msg_queue_len, sizeof(message));  // creating a queue for handling msgs between tasks


  // Create mutex before starting tasks and yielding it before starting 
  advanceToNextState= xSemaphoreCreateMutex();
  xSemaphoreTake(advanceToNextState, portMAX_DELAY);

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
  #ifdef DEBUG
    DebugSerial.print("Debug msg");
  #endif

}