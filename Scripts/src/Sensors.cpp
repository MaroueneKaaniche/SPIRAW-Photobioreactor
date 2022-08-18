#include "Sensors.h"

// acquiring temprature value from sensor using the dallas temperature library

float GetTemp()
{
    //add sensors.begin() in setup() -> main.cpp
    sensors.requestTemperatures(); 
    return(sensors.getTempCByIndex(0)); 
}

// acquiring turbidity level 
// the value of turbidity for clean water is 4.2/4.21 if not, use the potentiometer on amplifier the board to calibrate the sensor
float GetTurb()
{
        float volt = 0; 
        float ntu=0;
        for(int i=0; i<800; i++)    //doing 800 readings to reduce the noise from the measurments 
        {
            volt += ((float)analogRead(TurbPin)/1023)*5;
        }
        volt = volt/800;
        if(volt < 2.5){   // 3000 is the maximum value of turbidity and it correspends to 2.5 volt
        ntu = 3000;
        }else{
        ntu = -1120.4*(volt*volt)+5742.3*volt-4353.8;  //Y = -1120.4x2 + 5742.3x – 4352.9 with Y is the turbidity and X is the voltage
        }
    return(ntu);                                       //we can easily get the biomass given that 1NTU = 1 mg/L
    }

// acquiring pH level 


float GetpH()
{
    float calibration_value=21.34;                          // we alter this variable by using solution whose PH value is 7 and adding the difference (7-reading) to it
    int phval = 0; 
    int buffer_arr[10],temp;
    unsigned long int avgval; 

    for(int i=0;i<10;i++)                               // using the running average method
    { 
        buffer_arr[i]=analogRead(pHPin);
        delay(10);
    }
    for(int i=0;i<9;i++)                                //sort the analog from small to large
    {
        for(int j=i+1;j<10;j++)
        {
            if(buffer_arr[i]>buffer_arr[j])
            {
                temp=buffer_arr[i];
                buffer_arr[i]=buffer_arr[j];
                buffer_arr[j]=temp;
            }
        }
    }
    avgval=0;
    for(int i=2;i<8;i++)                                //take the average value of 6 center sample
        avgval+=buffer_arr[i];
    float volt=(float)avgval*5.0/1024/6;                //convert the analog into millivolt
    float ph_act = -5.70 * volt + calibration_value;    //convert the millivolt into pH value   
    return(ph_act);
}

void IRAM_ATTR pulseCounter()
{                                // this function is called upon external interrupts from the flowPin to count pulses, used later to get the flowRate and volume
  portENTER_CRITICAL(&synch);
  pulseCount++;
  portEXIT_CRITICAL(&synch);
}

float calibrationFactor=4.5;

void IRAM_ATTR GetFlow()
{                               // this function is called every second to calculate the flowRate (in liters/minute) and the volume in millilitres
  portENTER_CRITICAL_ISR(&timerMux);
  float flowRate = pulseCount / calibrationFactor;
  float flowMilliLitres = (flowRate / 60) * 1000;
  totalMilliLitres += flowMilliLitres;
  pulseCount=0;
  portEXIT_CRITICAL_ISR(&timerMux);
}

