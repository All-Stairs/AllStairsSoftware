#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 53
#define irLedPin 52          // IR Led on this pin
#define irSensorPin 26       // IR sensor on this pin

int irRead(int readPin, int triggerPin); //function prototype
int SensorValue;
int steps[30];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  pinMode(irSensorPin, INPUT);
  pinMode(irLedPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Program Starting");
  delay(100); 

  strip.begin();
  strip.setBrightness(30);
  strip.show(); // Initialize all pixels to 'off'

  for(int i = 0; i < 30; i++){
    steps[i] = 0;
    Serial.println(steps[i]);
  }
}

void loop() {
  SensorValue = irRead(irSensorPin, irLedPin);
 
  if(SensorValue == 1){
    steps[3] = 1;
    Serial.println("RED");
  }

  delay(20);
  lightTriggered();
}

void lightTriggered()
{
  for(int i = 0; i < 30; i++){
    if(steps[i] == 1){
      strip.setPixelColor(i, 0, 255, 0);
      steps[i]++;
    } else if(steps[i] == 50){
      steps[i] = 0;
      strip.setPixelColor(i, 0, 0, 0);
    } else if(steps[i] > 1 && steps[i] < 50){
      steps[i]++;
    }
  }
  strip.show();
}

/******************************************************************************
 * This function can be used with a panasonic pna4602m ir sensor
 * it returns a zero if something is detected by the sensor, and a 1 otherwise
 * The function bit bangs a 38.5khZ waveform to an IR led connected to the
 * triggerPin for 1 millisecond, and then reads the IR sensor pin to see if
 * the reflected IR has been detected
 ******************************************************************************/
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
