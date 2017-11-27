#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

//This starts the LED strip setup lines
#define PIN 6
#define irLedPin 52         // IR Led on this pin
#define irSensorPin 26      // IR Sensor on this pin

//This starts the Audio setup lines
#define VS1053_RX 2 //Connects to the RX pin on the VS1053
#define VS1053_RESET 9  //Connects to the reset pin on the VS1053

#define VS1053_BANK_DEFAULT 0x00
#define VS1053_BANK_DRUMS1 0x78
#define VS1053_BANK_DRUMS2 0x7F
#define VS1053_BANK_MELODY 0x79

//Set the instrument to an ocarina.
#define VS1053_GM1_OCARINA 80

#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0

SoftwareSerial VS1053_MIDI(0, 2);

int irRead(int readPin, int triggerPin); //function prototype
int SensorValue;   // Variable to read in the IR sensor
int steps[30];     // Array for 1 LED per stair (impractical)
int sounds[30];    // Similar to steps, but for sounds.  (TESTING)

int load            = 22;  // Connects to Load pin 1           74HC165
int clockEnablePin  = 24;  // Connects to Clock Enable pin 15  74HC165
//int dataIn          = 26;  // Connects to the Data pin 7       74HC165
int clockIn         = 28;  // Connects to the Clock pin 2      74HC165

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  //pinMode(irSensorPin, INPUT);
  pinMode(irLedPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Program Starting");
  delay(100); 

  pinMode(load, OUTPUT);
  pinMode(clockEnablePin, OUTPUT);
  pinMode(clockIn, OUTPUT);
  pinMode(irSensorPin, INPUT);


  strip.begin();
  strip.setBrightness(30);
  strip.show(); // Initialize all pixels to 'off'

  for(int i = 0; i < 30; i++){
    steps[i] = 0;
    Serial.println(steps[i]);
  }
  
  VS1053_MIDI.begin(31250);
  
  pinMode(VS1053_RESET, OUTPUT);
  digitalWrite(VS1053_RESET, LOW);
  delay(10);
  digitalWrite(VS1053_RESET, HIGH);
  delay(10);
  
  midiSetChannelBank(0, VS1053_BANK_MELODY);
  midiSetInstrument(0, VS1053_GM1_OCARINA);
  midiSetChannelVolume(0, 50);
  // End of Audio setup lines
}

void loop() {
  
      digitalWrite(load,LOW);
      delayMicroseconds(5);
      digitalWrite(load,HIGH);
      delayMicroseconds(5);
     
      digitalWrite(clockIn,HIGH);
      digitalWrite(clockEnablePin,LOW);
      byte incoming = shiftIn(irSensorPin, clockIn, LSBFIRST);
      Serial.println(incoming);
      digitalWrite(clockEnablePin,HIGH);
      delay(500);//slowing down for testing purposes
  
  
  SensorValue = irRead(bitRead(incoming,7), irLedPin);
    
  Serial.println("1st Bit");
  Serial.println(bitRead(incoming,7));

  Serial.println("Sensor Value");
  Serial.println(SensorValue);
  
  if(SensorValue == 1){
    steps[3] = 1;
    Serial.println("RED");
  }

  delay(20);
  lightTriggered();
/*
  Serial.print("Pin States:\r\n");
      Serial.println(incoming, BIN);
      
      //Printing individual bits of incoming byte
      Serial.print("DIP 1:\r");
      Serial.print(bitRead(incoming,7));
      Serial.print("\r\n");      
      Serial.print("DIP 2:\r");
      Serial.print(bitRead(incoming,6));
      Serial.print("\r\n");      
      Serial.print("DIP 3:\r");
      Serial.print(bitRead(incoming,5));
      Serial.print("\r\n");      
      Serial.print("DIP 4:\r");
      Serial.print(bitRead(incoming,4));
      Serial.print("\r\n");      
      Serial.print("DIP 5:\r");
      Serial.print(bitRead(incoming,3));
      Serial.print("\r\n");      
      Serial.print("DIP 6:\r");
      Serial.print(bitRead(incoming,2));
      Serial.print("\r\n");      
      Serial.print("DIP 7:\r");
      Serial.print(bitRead(incoming,1));
      Serial.print("\r\n");      
      Serial.print("DIP 8:\r");
      Serial.print(bitRead(incoming,0));
      Serial.print("\r");  
      */    
}

void lightTriggered()
{
  for(int i = 0; i < 30; i++){
    if(steps[i] == 1){
      for(int j = 0; j < 30; j++){
        strip.setPixelColor(j, 0, 255, 0);
      }
      steps[i]++;
    } else if(steps[i] == 50){
      for(int k = 0; k < 30; k++){
        strip.setPixelColor(k, 0, 0, 0);
      }
      steps[i] = 0;
    } else if(steps[i] > 1 && steps[i] < 50){
      steps[i]++;
    }
  }
  strip.show();
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

void midiSetInstrument(uint8_t chan, uint8_t inst) 
{
  if (chan > 15) return;
  inst --; // page 32 has instruments starting with 1 not 0 :(
  if (inst > 127) return;
  
  VS1053_MIDI.write(MIDI_CHAN_PROGRAM | chan);  
  VS1053_MIDI.write(inst);
}

void midiSetChannelVolume(uint8_t chan, uint8_t vol) 
{
  if (chan > 15) return;
  if (vol > 127) return;
  
  VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
  VS1053_MIDI.write(MIDI_CHAN_VOLUME);
  VS1053_MIDI.write(vol);
}

void midiSetChannelBank(uint8_t chan, uint8_t bank) 
{
  if (chan > 15) return;
  if (bank > 127) return;
  
  VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
  VS1053_MIDI.write((uint8_t)MIDI_CHAN_BANK);
  VS1053_MIDI.write(bank);
}

void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel) 
{
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  VS1053_MIDI.write(MIDI_NOTE_ON | chan);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vel);
}

void multiNoteOn(uint8_t chan, uint8_t m, uint8_t n, uint8_t vel)
{
  if (chan > 15) return;
  if (n > 127) return;
  if (m > 127) return;
  if (vel > 127) return;
  
  VS1053_MIDI.write(MIDI_NOTE_ON | chan);
  VS1053_MIDI.write(m);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vel);
}

void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel) 
{
  if (chan > 15) return;
  if (n > 127) return;
  if (vel > 127) return;
  
  VS1053_MIDI.write(MIDI_NOTE_OFF | chan);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vel);
}

void multiNoteOff(uint8_t chan, uint8_t m, uint8_t n, uint8_t vel)
{
  if (chan > 15) return;
  if (n > 127) return;
  if (m > 127) return;
  if (vel > 127) return;
  
  VS1053_MIDI.write(MIDI_NOTE_OFF | chan);
  VS1053_MIDI.write(m);
  VS1053_MIDI.write(n);
  VS1053_MIDI.write(vel);
}
