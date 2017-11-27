#include <Adafruit_VS1053.h>    // Music Maker Library
#include <Adafruit_NeoPixel.h>  // Neopixel Library
#include <SoftwareSerial.h>     // Serial Monitor Library
#ifdef __AVR__                  // _____________________________________________
#include <avr/power.h>          // _____________________________________________
#endif                          // End if these are not included

#define PIN 53              // LED Strip Control Pin
#define irLedPin 52         // IR Led on this pin
#define irSensorPin  26     // IR Sensor on this pin
#define irSensorPin2 24

#define VS1053_RX 2           //Connects to the RX pin on the Music Maker
#define VS1053_RESET 9        //Connects to the reset pin on the Music Maker

#define VS1053_BANK_DEFAULT 0x00
#define VS1053_BANK_DRUMS1 0x78
#define VS1053_BANK_DRUMS2 0x7F
#define VS1053_BANK_MELODY 0x79

#define VS1053_GM1_OCARINA 80 //Set the instrument to an ocarina.

#define MIDI_NOTE_ON  0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CHAN_MSG 0xB0
#define MIDI_CHAN_BANK 0x00
#define MIDI_CHAN_VOLUME 0x07
#define MIDI_CHAN_PROGRAM 0xC0

SoftwareSerial VS1053_MIDI(0, 2);

int irRead(int readPin, int triggerPin); // Function Prototype
int SensorValue;                         // Variable to read in the IR sensor
int SensorValue2;
int steps[30];                           // Array for 1 LED per stair (impractical)
int sounds[30];                          // Similar to steps, but for sounds.  (TESTING)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);   // Sets up the Neopixel strip

void setup() {                          // Initialize Function
  
  pinMode(irSensorPin, INPUT);          // Sets the Sensor Pin to Input
  pinMode(irSensorPin2, INPUT);
  pinMode(irLedPin, OUTPUT);            // Sets the LED Pin to Output
  Serial.begin(9600);                   // Starts the Serial Monitor
  Serial.println("Program Starting");   // Tells the User that the program is starting
  delay(100);                           // Delay's for a 10th of a second

  strip.begin();                        // Sets up the LED strip
  strip.setBrightness(30);              // Sets the brightness for the strip
  strip.show();                         // Initialize all pixels to 'off'

  for(int i = 0; i < 30; i++){          // Fills the Steps Array with 0's
    steps[i] = 0;
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
  
}

void loop() {                                    // This is the Main Loop

  SensorValue = irRead(irSensorPin, irLedPin);   // This function reads in the Sensor Value
  SensorValue2 = irRead(irSensorPin2, irLedPin);

  if(SensorValue == 1 && SensorValue2 == 1){                          // If the Sensor Value is HIGH, then enter this.
    steps[3] = 1;                                // In the Steps array, 3 = High
    steps[6] = 1;                                // In the Steps array, 3 = High
    midiNoteOn(0, 60, 127);                      // Turns the note ON
    midiNoteOn(0, 90, 127);                      // Turns the note ON
    lightTriggered();                            // Calls the Light Triggered Function
    delay(1000);                                 // Delay for a second
    lightsOut();                                 // Turn off the Lights
    midiNoteOff(0,60,127);                       // Turn the note OFF
    midiNoteOff(0,90,127);                       // Turn the note OFF
    delay(500);                                  // Delay for half a second
  }else if (SensorValue2 == 1 && SensorValue == 0){
    steps[6] = 1;                                // In the Steps array, 3 = High
    midiNoteOn(0, 90, 127);                      // Turns the note ON
    lightTriggered();                            // Calls the Light Triggered Function
    delay(1000);                                 // Delay for a second
    lightsOut();                                 // Turn off the Lights
    midiNoteOff(0,90,127);                       // Turn the note OFF
    delay(500);                                  // Delay for half a second
  }else if (SensorValue = 1 && SensorValue2 == 0){
    steps[3] = 1;                                // In the Steps array, 3 = High
    midiNoteOn(0, 60, 127);                      // Turns the note ON
    lightTriggered();                            // Calls the Light Triggered Function
    delay(1000);                                 // Delay for a second
    lightsOut();                                 // Turn off the Lights
    midiNoteOff(0,60,127);                       // Turn the note OFF
    delay(500);                                  // Delay for half a second
  }

}


void lightTriggered()                           // Light Triggered Function
{
  for(int i = 0; i < 30; i++){
    if(steps[i] == 1){
         for(int j = 0; j <30; j++){
        strip.setPixelColor(j, 0, 255, 0);
        }
      steps[i]++;
    } else if(steps[i] == 50){
      steps[i] = 0;
      strip.setPixelColor(i, 0, 0, 0);
    } else if(steps[i] > 1 && steps[i] < 50){
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

int irRead(int readPin, int triggerPin)    // Reads in the IR Receiver Value
{
  int halfPeriod = 13;                     // One period at 38.5khZ is aproximately 26 microseconds
  int cycles = 38;                         // 26 microseconds * 38 is more or less 1 millisecond
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
