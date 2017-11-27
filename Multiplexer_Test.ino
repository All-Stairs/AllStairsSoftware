//    74HC165
           
             // Pin 1 -  Load
             // Pin 2 -  Clock In
             // Pin 3 -  Input 4
             // Pin 4 -  Input 5
             // Pin 5 -  Input 6
             // Pin 6 -  Input 7
             // Pin 7 -  Data
             // Pin 8 -  Negative (Ground)
             // Pin 9 -  (Not Used)
             // Pin 10 - (Not Used)
             // Pin 11 - Input 0
             // Pin 12 - Input 1
             // Pin 13 - Input 2
             // Pin 14 - Input 3
             // Pin 15 - Clock Enable (CE)
             // Pin 16 - Vcc Voltage IN (+)

      int load            = 2;  // Connects to Load pin 1           74HC165
      int clockEnablePin  = 5;  // Connects to Clock Enable pin 15  74HC165
      int dataIn          = 4;  // Connects to the Data pin 7       74HC165
      int clockIn         = 3;  // Connects to the Clock pin 2      74HC165
   
    void setup()
    {
        Serial.begin(9600);

        
        pinMode(load, OUTPUT);
        pinMode(clockEnablePin, OUTPUT);
        pinMode(clockIn, OUTPUT);
        pinMode(dataIn, INPUT);
    }
    
    void loop()
    {
      
      digitalWrite(load,LOW);
      delayMicroseconds(5);
      digitalWrite(load,HIGH);
      delayMicroseconds(5);
     
      digitalWrite(clockIn,HIGH);
      digitalWrite(clockEnablePin,LOW);
      byte incoming = shiftIn(dataIn, clockIn, LSBFIRST);
      digitalWrite(clockEnablePin,HIGH);
            
      
      //Printing individual bits of incoming byte
      Serial.print("DIP States:\r\n");
      Serial.print(bitRead(incoming,7));
      Serial.print(bitRead(incoming,6));
      Serial.print(bitRead(incoming,5));
      Serial.print(bitRead(incoming,4));
      Serial.print(bitRead(incoming,3));
      Serial.print(bitRead(incoming,2));
      Serial.print(bitRead(incoming,1));
      Serial.print(bitRead(incoming,0));  
      Serial.print("\r\n");
      
      delay(500);//slowing down for testing purposes
      }
