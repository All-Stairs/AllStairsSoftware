/*
   SensorSubsystem.cpp - source file for All Stairs SensorSubsystem library
   Adapted by Fernando Esquivel using various sources
*/

#include "Arduino.h"
#include "SensorSubsystem.h"

SensorSubsystem::SensorSubsystem (int irLed, int load, int clockEnable, int dataIn, int clockIn)
{
     irLedPin = irLed;
     loadPin = load;
     clockEnablePin = clockEnable;
     dataInPin = dataIn;
     clockInPin = clockIn;

     pinMode (irLedPin, OUTPUT);
     pinMode (loadPin, OUTPUT);
     pinMode (clockEnablePin, OUTPUT);
     pinMode (dataInPin, INPUT);
     pinMode (clockInPin, OUTPUT);

}

byte SensorSubsystem::IRMultiplexer() {
  //Serial.println("GETTING MUX VALUE");
  //setup for IR
  int halfPeriod = 13;                     // One period at 38.5khZ is aproximately 26 microseconds
  int cycles = 38;                         // 26 microseconds * 38 is more or less 1 millisecond
  int i;
  byte incoming;
  
  //setup for multiplexer
  digitalWrite(loadPin,LOW);
  delayMicroseconds(5);
   //Cycles for IR triggering due to 38 kHz signal  
  for (i=0; i <=cycles; i++)
  {
    digitalWrite(irLedPin, HIGH); 
    delayMicroseconds(halfPeriod);
    digitalWrite(irLedPin, LOW); 
    delayMicroseconds(halfPeriod - 1);     // - 1 to make up for digitaWrite overhead    
  }  
  digitalWrite(loadPin,HIGH);
  delayMicroseconds(5);  
  digitalWrite(clockInPin,HIGH);    
  digitalWrite(clockEnablePin,LOW); //*MULTIPLEXER SHOULD BE READY TO RECEIVE DATA AT THIS POINT??*   
  incoming = shiftIn(dataInPin, clockInPin, LSBFIRST); //SHOULD WE HAVE DATA AT THIS POINT? 
  digitalWrite(clockEnablePin,HIGH);    
  
  return incoming;

}