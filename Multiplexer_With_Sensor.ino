//    74HC165
           
             // Pin 1 -  Load
             // Pin 2 -  Clock In
             // Pin 3 -  Input 4
             // Pin 4 -  Input 5
             // Pin 5 -  Input 6
             // Pin 6 -  Input 7
             // Pin 7 -  (Not Used)
             // Pin 8 -  Negative (Ground)
             // Pin 9 -  Data
             // Pin 10 - (Not Used)
             // Pin 11 - Input 0
             // Pin 12 - Input 1
             // Pin 13 - Input 2
             // Pin 14 - Input 3
             // Pin 15 - Clock Enable (CE)
             // Pin 16 - Vcc Voltage IN (+)


#ifdef __AVR__
#include <avr/power.h>
#endif

#define irLedPin      7     // IR Led on this pin
#define irSensorPin   4     // IR sensor on this pin


int load            = 2;  // Connects to Load pin 1           74HC165
int clockIn         = 3;  // Connects to the Clock pin 2      74HC165
int clockEnablePin  = 5;  // Connects to Clock Enable pin 15  74HC165

int irRead(int readPin, int triggerPin); //function prototype
int SensorValue;

    void setup()
    {
        Serial.begin(9600);

        
        pinMode(load, OUTPUT);
        pinMode(clockEnablePin, OUTPUT);
        pinMode(clockIn, OUTPUT);
        pinMode(irSensorPin, INPUT);

        pinMode(irLedPin, OUTPUT);
        Serial.begin(9600);
        Serial.println("Program Starting");
        delay(100); 

    }
    
    void loop(){
      
      digitalWrite(load,LOW);
      delayMicroseconds(5);
      digitalWrite(load,HIGH);
      delayMicroseconds(5);
     
      digitalWrite(clockIn,HIGH);
      digitalWrite(clockEnablePin,LOW);
      byte incoming = shiftIn(irSensorPin, clockIn, LSBFIRST);
      digitalWrite(clockEnablePin,HIGH);


      SensorValue = irRead(irSensorPin, irLedPin);
      Serial.println(SensorValue);
/*
      if(bitRead(incoming,7) == 1){
        Serial.println("ONE");
      } else if(bitRead(incoming,6) == 1){
        Serial.println("TWO");
      }
      
      Serial.print(bitRead(incoming,7));
      Serial.print(bitRead(incoming,6));
      Serial.print(bitRead(incoming,5));
      Serial.print(bitRead(incoming,4));
      Serial.print(bitRead(incoming,3));
      Serial.print(bitRead(incoming,2));
      Serial.print(bitRead(incoming,1));
      Serial.print(bitRead(incoming,0));  
      Serial.print("\r\n");
      */
}


int irRead(int readPin, int triggerPin)
{
  int halfPeriod = 13; //one period at 38.5khZ is aproximately 26 microseconds
  int cycles = 38; //26 microseconds * 38 is more or less 1 millisecond
  int i;
  for (i=0; i <=cycles; i++)
  {
    digitalWrite(triggerPin, HIGH); 
    delayMicroseconds(halfPeriod);
    digitalWrite(triggerPin, LOW); 
    delayMicroseconds(halfPeriod - 1);     // - 1 to make up for digitaWrite overhead    
  }
  return digitalRead(readPin);
}
