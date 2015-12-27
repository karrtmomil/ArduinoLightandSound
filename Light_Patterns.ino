/*
 * Code to cycle through LED light patterns from a pushbutton
 * Karl Akert 2015/12/25
 */

#include "pitches.h"

const int patternButtonPin = 2; // use digital imput pin 2 for the button pin to change the pattern
volatile bool patternButtonState = false; // state of button
const int controlButtonPin = 3; // use digital imput pin 3 for the button pin to swap the analog inputs
volatile bool controlState = true; // state of button
int LED0 = 6; // Use digital pin 6 to drive the white LED
int LED1 = 9; // Use digital pin 9 to drive the yellow LED
int LED2 = 10; // Use digital pin 10 to drive the green LED
int LED3 = 5; // Use digital pin 5 to drive the red LED
int LED4 = 11; // Use digital pin 11 to drive the blue LED
int brightnessPin = A0; // analog input pin to control brightness
int speedPin = A1; // analog input to control period
int ledBrightness = 100; // brightness for LEDs
const int maxBrightness = 255; // Max brightness for LEDs
const int minBrightness = 10; // Min Brightness for LEDs
int flashPeriod = 250; // Delay for leds
const int maxPeriod = 3000; // maximum period for delay
const int minPeriod = 200; // minimum period for delay
int pattern = 0; // control for what pattern to display
int i = 0; // counter for FOR loops
int buzzerPin = 8; // Pin for tone output
float noteGap = 0.3;

void setup() {
  // initialize the pushbutton pins as inputs:
  pinMode(patternButtonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(patternButtonPin), patternButtonPress, FALLING);
  pinMode(controlButtonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(controlButtonPin), controlButtonPress, FALLING);

  // initialize digital pins as output:
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  // initialize the serial port:
  Serial.begin(9600); // start serial for output

  
  tone(buzzerPin, NOTE_C6, flashPeriod);
  delay(flashPeriod);
  tone(buzzerPin, NOTE_D6, flashPeriod);
  delay(flashPeriod);
  tone(buzzerPin, NOTE_E6, flashPeriod);
  delay(flashPeriod);
  tone(buzzerPin, NOTE_F6, flashPeriod);
  delay(flashPeriod);
  tone(buzzerPin, NOTE_G6, flashPeriod);
  delay(flashPeriod);
  tone(buzzerPin, NOTE_A6, flashPeriod);
  delay(flashPeriod);
  tone(buzzerPin, NOTE_B6, flashPeriod);
  delay(flashPeriod);
  tone(buzzerPin, NOTE_C7, flashPeriod);
  delay(flashPeriod);
  
}

void loop() {

  switch (pattern) {
    case 0:
      defaultPattern();
      break;
    case 1:
      abbysPattern();
      break;
    case 2:
      momsPattern();
      break;
    case 3:
      imperialMarch();
      break;
    default:
      pattern = 0;
      break;
  }
  
  if (patternButtonState) {
    pattern++;
    flashAll(flashPeriod / 3);
    flashAll(flashPeriod / 3);
    patternButtonState = false;
  }
  
  /*Serial.print("Current pattern: ");
  Serial.println(pattern);
  Serial.print("Current Brightness: ");
  Serial.println(ledBrightness);
  Serial.print("Current Period: ");
  Serial.println(flashPeriod);
  Serial.print("Control State: ");
  Serial.println(controlState);
  Serial.println("-------------------------------------------------------");
  */
}

void flashAll(int tDelay) {
  updateInputs();
  analogWrite(LED0, ledBrightness);
  analogWrite(LED1, ledBrightness);
  analogWrite(LED2, ledBrightness);
  analogWrite(LED3, ledBrightness);
  delay(tDelay);
  updateInputs();
  analogWrite(LED0, LOW);
  analogWrite(LED1, LOW);
  analogWrite(LED2, LOW);
  analogWrite(LED3, LOW);
  delay(tDelay);
}

void defaultPattern() {
  int patternOrder[] = {NOTE_C7,NOTE_D7,NOTE_E7,NOTE_F7};
  int durations[] = {4,4,4,4};
  int patternLen = 4;
  playSerialSequence(patternOrder, durations, patternLen);
}

void abbysPattern() {
  int patternOrder[] = {NOTE_E7,NOTE_D7,NOTE_F7,NOTE_C7};
  int durations[] = {4,4,4,4};
  int patternLen = 4;
  playSerialSequence(patternOrder, durations, patternLen);
}

void altPattern() {
  flashDoubleLED(LED2, LED3, flashPeriod);
  if (patternButtonState) return;
  flashDoubleLED(LED0, LED1, flashPeriod);
}
void insideOutPattern() {
  flashDoubleLED(LED0, LED3, flashPeriod);
  if (patternButtonState) return;
  flashDoubleLED(LED1, LED2, flashPeriod);
}

void momsPattern() {
  int patternOrder[] = {NOTE_C7,NOTE_E7,NOTE_D7,NOTE_F7, 
                        NOTE_D7,NOTE_D7,NOTE_D7,NOTE_E7,
                        NOTE_C7,NOTE_E7,NOTE_D7,NOTE_F7,
                        NOTE_E7,NOTE_E7,NOTE_E7,NOTE_F7,
                        NOTE_C7,NOTE_E7,NOTE_D7,NOTE_F7, 
                        NOTE_F7,NOTE_F7,NOTE_F7,NOTE_G7,
                        NOTE_C7,NOTE_E7,NOTE_D7,NOTE_F7,
                        NOTE_G7,NOTE_G7,NOTE_G7,NOTE_A7};
  int durations[] = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
  int patternLen = 32;
  playSerialSequence(patternOrder, durations, patternLen);
}

void imperialMarch() {
  int patternOrder[] {NOTE_A3, NOTE_A3, NOTE_A3, NOTE_F3, NOTE_C4, NOTE_A3, NOTE_F3, NOTE_C4, NOTE_A3,
                      NOTE_E4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_C4, NOTE_GS3, NOTE_F3, NOTE_C4, NOTE_A3};
  int durations[] = {4,4,4,16/3,16,4,15/3,16,2,
                     4,4,4,16/3,16,4,15/3,16,2};
  int patternLen = 18;
  playSerialSequence(patternOrder, durations, patternLen);
}

void playSerialSequence(int patternL[], int durations[], int len) {
  for (i = 0; i < len; i++) {
    int note = patternL[i];
    int duration = durations[i];
    tone(buzzerPin, note, flashPeriod / duration);
    switch (note) {
      case NOTE_B0:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_C1:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_CS1:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_D1:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_DS1:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_E1:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_F1:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_FS1:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_G1:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_GS1:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_A1:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_AS1:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_B1:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_C2:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_CS2:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_D2:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_DS2:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_E2:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_F2:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_FS2:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_G2:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_GS2:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_A2:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_AS2:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_B2:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_C3:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_CS3:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_D3:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_DS3:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_E3:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_F3:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_FS3:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_G3:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_GS3:
        flashDoubleLED(LED0, LED1, flashPeriod / duration);
        break;
      case NOTE_A3:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_AS3:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_B3:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_C4:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_CS4:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_D4:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_DS4:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_E4:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_F4:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_FS4:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_G4:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_GS4:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_A4:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_AS4:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_B4:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_C5:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_CS5:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_D5:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_DS5:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_E5:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_F5:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_FS5:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_G5:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_GS5:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_A5:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_AS5:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_B5:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_C6:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_CS6:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_D6:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_DS6:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_E6:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_F6:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_FS6:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_G6:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_GS6:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_A6:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_AS6:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_B6:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_C7:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_CS7:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_D7:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_DS7:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_E7:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_F7:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_FS7:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_G7:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_GS7:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_A7:
        flashSingleLED(LED1, flashPeriod / duration);
        break;
      case NOTE_AS7:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_B7:
        flashSingleLED(LED2, flashPeriod / duration);
        break;
      case NOTE_C8:
        flashSingleLED(LED3, flashPeriod / duration);
        break;
      case NOTE_CS8:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
      case NOTE_D8:
        flashSingleLED(LED0, flashPeriod / duration);
        break;
      case NOTE_DS8:
        delay( (flashPeriod / duration) * noteGap); // todo: change this delat to flash 2 LEDs based on which sharp note it is
        break;
    }
    /*switch (patternL[i]) {
      case 1:
        tone(buzzerPin, NOTE_C7, flashPeriod);
        flashSingleLED(LED0);
        break;
      case 2:
        tone(buzzerPin, NOTE_D7, flashPeriod);
        flashSingleLED(LED1);
        break;
      case 3:
        tone(buzzerPin, NOTE_E7, flashPeriod);
        flashSingleLED(LED2);
        break;
      case 4:
        tone(buzzerPin, NOTE_F7, flashPeriod);
        flashSingleLED(LED3);
        break;
    }*/
  }
}

void flashSingleLED(int LED, int duration) {
  updateInputs();
  if (patternButtonState) return;
  analogWrite(LED, ledBrightness);
  delay(duration);
  if (patternButtonState) return;
  analogWrite(LED, LOW);
  delay(duration * noteGap);
}

void flashDoubleLED(int LED1, int LED2, int duration) {
  updateInputs();
  if (patternButtonState) return;
  analogWrite(LED1, ledBrightness);
  analogWrite(LED2, ledBrightness);
  delay(duration);
  if (patternButtonState) return;
  analogWrite(LED1, LOW);
  analogWrite(LED2, LOW);
  delay(duration * noteGap);
}

void updateInputs() {
  if (controlState) {
    ledBrightness = analogRead(brightnessPin) * ((maxBrightness - minBrightness) / 1023.0) + minBrightness;
    flashPeriod = analogRead(speedPin) * ((maxPeriod - minPeriod) / 1023.0) + minPeriod;
    digitalWrite(LED4, LOW);
  }
  else {
    ledBrightness = analogRead(speedPin) * ((maxBrightness - minBrightness) / 1023.0) + minBrightness;
    flashPeriod = maxPeriod - analogRead(brightnessPin) * ((maxPeriod - minPeriod) / 1023.0);
    digitalWrite(LED4, ledBrightness);
  }
}

void patternButtonPress() {
  patternButtonState = true;
}

void controlButtonPress() {
  controlState = !controlState;
}

