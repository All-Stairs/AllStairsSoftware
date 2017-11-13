#include <SoftwareSerial.h>

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

// Define the pins on the mega for the buttons and LED
const int buttonOne = 53; 
const int buttonTwo = 49; 
const int buttonThree = 45;
const int ledPin = 51;

//Keep track of whether the pins are high or low
int buttonOneState = 0; 
int buttonTwoState = 0;
int buttonThreeState = 0;

void setup() 
{
  Serial.begin(9600);
   //Set the pin for the buttons as inputs
  pinMode(buttonOne, INPUT); 
  pinMode(buttonTwo, INPUT);
  pinMode(buttonThree, INPUT);
  //pinMode(ledPin, OUTPUT);
  
  VS1053_MIDI.begin(31250);
  
  pinMode(VS1053_RESET, OUTPUT);
  digitalWrite(VS1053_RESET, LOW);
  delay(10);
  digitalWrite(VS1053_RESET, HIGH);
  delay(10);
  
  midiSetChannelBank(0, VS1053_BANK_MELODY);
  midiSetInstrument(0, VS1053_GM1_OCARINA);
  midiSetChannelVolume(0, 50);
}

void loop() 
{
  buttonOneState = digitalRead(buttonOne);
  buttonTwoState = digitalRead(buttonTwo);
  buttonThreeState = digitalRead(buttonThree);

   if(buttonOneState == HIGH && buttonTwoState == HIGH)
  {
    midiNoteOn(0, 60, 127);
    midiNoteOn(0, 50, 127);
    delay(1000);
    midiNoteOff(0, 60, 127);
    midiNoteOff(0, 50, 127);
  }
  else if(buttonOneState == HIGH)
  {
    //Play a note, wait, and turn the note off
    midiNoteOn(0, 60, 127);
    delay(1000);
    midiNoteOff(0, 60, 127);
  }
  else if(buttonTwoState == HIGH)
  {
    //Play a note, wait, and turn the note off
    midiNoteOn(0, 50, 127);
    delay(1000);
    midiNoteOff(0, 50, 127);
  }
  else if(buttonThreeState == HIGH)
  {
    midiNoteOn(0, 40, 127);
    delay(1000);
    midiNoteOff(0, 40, 127);
  }
  else if(buttonOneState == HIGH && buttonTwoState == HIGH && buttonThreeState == HIGH)
  {
    midiNoteOn(0, 60, 127);
    midiNoteOn(0, 50, 127);
    midiNoteOn(0, 40, 127);
    delay(1000);
    midiNoteOff(0, 60, 127);
    midiNoteOff(0, 50, 127);
    midiNoteOff(0, 40, 127);
  }
  else if(buttonOneState == HIGH && buttonThreeState == HIGH)
  {
    midiNoteOn(0, 60, 127);
    midiNoteOn(0, 40, 127);
    delay(1000);
    midiNoteOff(0, 60, 127);
    midiNoteOff(0, 40, 127);
  }
  else if(buttonTwoState == HIGH && buttonThreeState == HIGH)
  {
    midiNoteOn(0, 50, 127);
    midiNoteOn(0, 40, 127);
    delay(1000);
    midiNoteOff(0, 50, 127);
    midiNoteOff(0, 40, 127);
  }
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
