/*
     SensorSubsystem.h - Library for the sensor subsystem of the All-Stairs project
     Adapted by Fernando Esquivel using various sources 
*/

#ifndef SensorSubsystem_h
#define SensorSubsystem_h

#include "Arduino.h"

class SensorSubsystem
{
   public:
     SensorSubsystem (int irLed, int load, int clockEnable, int dataIn, int clockIn);
     byte IRMultiplexer ();

   private:
     int irLedPin;
     int loadPin;  
     int clockEnablePin;   
     int dataInPin;
     int clockInPin;
};

#endif