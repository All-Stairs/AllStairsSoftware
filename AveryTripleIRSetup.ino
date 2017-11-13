#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define irLedPin 4          // IR Led on this pin
#define irSensorPin1 11       // IR sensor on this pin
#define irSensorPin2 10
#define irSensorPin3 9

int irRead(int readPin, int triggerPin); //function prototype
int SensorValue1;
int SensorValue2;
int SensorValue3;
int steps[30];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  pinMode(irSensorPin1, INPUT);
  pinMode(irSensorPin2, INPUT);
  pinMode(irSensorPin3, INPUT);
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
  SensorValue1 = irRead(irSensorPin1, irLedPin);
  SensorValue2 = irRead(irSensorPin2, irLedPin);
  SensorValue3 = irRead(irSensorPin3, irLedPin);
  if(SensorValue1 == 1){
    steps[3] = 1;
    Serial.println("RED");
  }
  if(SensorValue2 == 1){
    steps[4] = 1;
    Serial.println("GREEN");
  }
  if(SensorValue3 == 1){
    steps[5] = 1;
    Serial.println("BLUE");
  }

  delay(20);
  lightTriggered2();
}

void lightTriggered2()
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
