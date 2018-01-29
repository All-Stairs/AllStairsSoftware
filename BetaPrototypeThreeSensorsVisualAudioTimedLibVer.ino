#include <SensorSubsystem.h>
//BASED ON WIRING AS OF JAN 12 2013
//Jan 19 2018 - Code now works with three sensors

/*BIT MAP (MULTIPLEXER TO SERIAL OUTPUT)
Multiplexer Pin             Serial Bit (Based on most recent output schemem LSBFIRST)
A                            7
B                            6
C                            5
D                            4 not sure
E                            3 not sure
F                            2 not sure
G                            1 not sure
H                            0 not sure

*/
#include <SoftwareSerial.h>     // Serial Monitor Library
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>    //Music Maker library
#include <Adafruit_NeoPixel.h>  //Neopixel Library
#ifdef __AVR__                  // _____________________________________________
#include <avr/power.h>          // _____________________________________________
#endif                          // End if these are not included

#define NeoPixelPin 30      //Pin that controls NeoPixel strip
//#define interruptPin 21

#define VS1053_RX 2           //Connects to the RX pin on the Music Maker
#define VS1053_RESET 9        //Connects to the reset pin on the Music Maker

#define VS1053_BANK_DEFAULT 0x00
#define VS1053_BANK_DRUMS1 0x78
#define VS1053_BANK_DRUMS2 0x7F
#define VS1053_BANK_MELODY 0x79

#define VS1053_GM1_OCARINA 1 //Set the instrument to an ocarina.

#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0

SoftwareSerial VS1053_MIDI(0, 2);


int irLedPin = 37;
int load            = 39;  // Connects to Load pin 1           74HC165
int clockEnablePin  = 47;  // Connects to Clock Enable pin 15  74HC165
int dataIn          = 43;  // Connects to the Data pin 7       74HC165
int clockIn = 41;  // Connects to the Clock pin 2      74HC165
byte stepData; //byte to be returned from IR/Multiplexer method
int steps [30]; //array to keep track of which sensor was triggered
int debounces [30]; //array for debouncing values so we don't immediately re-trigger 
SensorSubsystem sensor (irLedPin, load, clockEnablePin, dataIn, clockIn);

int sounds[30]; //similar to steps, but for sounds

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NeoPixelPin, NEO_GRB + NEO_KHZ800);   // Sets up the Neopixel strip
 
 
void setup()
{
    Serial.begin(9600);    
            
    strip.begin();                        // Sets up the LED strip
    strip.setBrightness(30);              // Sets the brightness for the strip
    strip.show();                         // Initialize all pixels to 'off'

    for(int i = 0; i < 30; i++){          // Fills the Steps Array with 0's
      steps[i] = 0;
    }
    
        for(int i = 0; i < 30; i++){          // Fills the Debounces Array with 0's
      debounces[i] = 0;
    }
    
      VS1053_MIDI.begin(31250);             // Opens the MIDI library to pull sounds
  
  pinMode(VS1053_RESET, OUTPUT);        // Sets the Reset Pin to be Output
  digitalWrite(VS1053_RESET, LOW);      // Sets the Reset Low
  delay(10);                            // Delay for a bit
  digitalWrite(VS1053_RESET, HIGH);     // Sets the Reset High
  delay(10);                            // Delay for a bit
  
  midiSetChannelBank(0, VS1053_BANK_MELODY); //__________________________________
  midiSetInstrument(0, VS1053_GM1_OCARINA);  //__________________________________
  midiSetChannelVolume(0, 50);               // This sets the volume for Channel 0
  
 // attachInterrupt(digitalPinToInterrupt(interruptPin), doThing, CHANGE);    
         
    Serial.println("Program Starting");   // Tells the User that the program is starting
    delay(100);                           // Delay's for a 10th of a second
}

void loop() 
{  
  stepData = sensor.IRMultiplexer();
  delay(50);//slowing down for testing purposes  
    if (bitRead(stepData,7) == 0)
  {
    Serial.println("SENSOR 1 TRIGGERED");    
    if (debounces[0] == 0)
    {
          steps[0] = 1;
          debounces[0] = 1;
    }
  }

  if (bitRead(stepData,6) == 0)
  {
    Serial.println("SENSOR 2 TRIGGERED");
        if (debounces[1] == 0)
    {
          steps[1] = 1;
          debounces[1] = 1;
    }
  }  
  
//  if (bitRead(stepData,5) == 0)
//  {
//    Serial.println("SENSOR 3 TRIGGERED");
//    if (debounces[2] == 0)
//    {
//          steps[2] = 1;
//          debounces[2] = 1;
//    }
//  }
    lightTriggered(); //lights AND sound
    
  Serial.println(steps[0]);  
  Serial.println(steps[1]);  
//  Serial.println(steps[2]);  


      
}
  
  
//**************FUNCTIONS**********************  

void lightTriggered()                           // Light Triggered Function
{
  for(int i = 0; i < 30; i++){
    if(steps[i] == 1){
      
      if (i == 0)
      {
           for(int j = 0; j <10; j++){
        strip.setPixelColor(j, 0, 255, 0);
        }
        midiNoteOn(0, 60, 127); 
      }
      
     if (i == 1)
     {
                for(int j = 10; j <20; j++){
        strip.setPixelColor(j, 0, 0, 255);
        }
        midiNoteOn(0, 70, 127); 
     }
     
     if (i == 2)
     {
                for(int j = 20; j <30; j++){
        strip.setPixelColor(j, 255, 0, 0);
        }
        midiNoteOn(0, 80, 127); 
     }
      
      steps[i]++;
    } else if(steps[i] == 30){
      steps[i] = 0;
      debounces[i] = 0;
      //strip.setPixelColor(i, 0, 0, 0);
      if (i == 0)
      {
           for(int j = 0; j <10; j++){
        strip.setPixelColor(j, 0, 0, 0);
        }
        midiNoteOff(0, 60, 127); 
      }
      
     if (i == 1)
     {
                for(int j = 10; j <20; j++){
        strip.setPixelColor(j, 0, 0, 0);
        }
        midiNoteOff(0, 70, 127); 
     }
     
     if (i == 2)
     {
                for(int j = 20; j <30; j++){
        strip.setPixelColor(j, 0, 0, 0);
        }
        midiNoteOff(0, 80, 127); 
     }      
      
    } else if(steps[i] > 1 && steps[i] < 30){
      steps[i]++;
    }
  }
  strip.show();                                // Tells the strip to turn ON
}

void lightsOut()                               // Lights Out Function
{
          for(int j = 0; j <30; j++){
        strip.setPixelColor(j, 0, 0, 0);
          }  
        strip.show();                                // Initialize all pixels to 'off'
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

  Serial.println("Sound should be on/r/n");
  
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

