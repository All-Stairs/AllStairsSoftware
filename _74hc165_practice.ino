//    74HC595                           74HC165
           
             // Pin 1 -  LED 1                    Pin 1 -  Load
             // Pin 2 -  LED 2                    Pin 2 -  Clock
             // Pin 3 -  LED 3                    Pin 3 -  Input 4
             // Pin 4 -  LED 4                    Pin 4 -  Input 5
             // Pin 5 -  LED 5                    Pin 5 -  Input 6
             // Pin 6 -  LED 6                    Pin 6 -  Input 7
             // Pin 7 -  LED 7                    Pin 7 -  Data
             // Pin 8 -  Negative (Ground)        Pin 8 -  Negative (Ground)
             // Pin 9 -  (Not Used) Serial out    Pin 9 -  (Not Used)
             // Pin 10 - MR Master Reset (+)      Pin 10 - (Not Used)
             // Pin 11 - SHCP CLOCK               Pin 11 - Input 0
             // Pin 12 - STCP LATCH               Pin 12 - Input 1
             // Pin 13 - OE Output Enable (-)     Pin 13 - Input 2
             // Pin 14 - DS DATA                  Pin 14 - Input 3
             // Pin 15 - LED 0                    Pin 15 - Clock Enable (CE)
             // Pin 16 - Voltage IN (+)           Pin 16 - Vcc Voltage IN (+)

      int load            = 7;  // Connects to Load pin 1           74HC165
      int clockEnablePin  = 4;  // Connects to Clock Enable pin 15  74HC165
      int dataIn          = 5;  // Connects to the Data pin 7       74HC165
      int clockIn         = 6;  // Connects to the Clock pin 2      74HC165
   
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
            
      Serial.print("Pin States:\r\n");
      Serial.println(incoming, BIN);
      
      //Printing individual bits of incoming byte
      Serial.print("DIP 1:\r\n");
      Serial.print(bitRead(incoming,7));
      Serial.print("\r\n");      
      Serial.print("DIP 2:\r\n");
      Serial.print(bitRead(incoming,6));
      Serial.print("\r\n");      
      Serial.print("DIP 3:\r\n");
      Serial.print(bitRead(incoming,5));
      Serial.print("\r\n");      
      Serial.print("DIP 4:\r\n");
      Serial.print(bitRead(incoming,4));
      Serial.print("\r\n");      
      Serial.print("DIP 5:\r\n");
      Serial.print(bitRead(incoming,3));
      Serial.print("\r\n");      
      Serial.print("DIP 6:\r\n");
      Serial.print(bitRead(incoming,2));
      Serial.print("\r\n");      
      Serial.print("DIP 7:\r\n");
      Serial.print(bitRead(incoming,1));
      Serial.print("\r\n");      
      Serial.print("DIP 8:\r\n");
      Serial.print(bitRead(incoming,0));
      Serial.print("\r\n");      
      
      delay(500);//slowing down for testing purposes
      }
