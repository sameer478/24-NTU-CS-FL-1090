#include <Arduino.h>

const int led1Pin = 12;
const int led2Pin = 13;

// PWM properties
const int freq1 = 1000;
const int freq2 = 5000;
const int resolution = 8; // 8-bit resolution (0-255)

int dutyCycle1 = 0;
int dutyCycle2 = 0;
int fadeAmount1 = 5;
int fadeAmount2 = 3;

void setup() {
  Serial.begin(115200);
  
  // New API: ledcAttach(pin, frequency, resolution)
  ledcAttach(led1Pin, freq1, resolution);
  ledcAttach(led2Pin, freq2, resolution);
}

void loop() {
  // New API: ledcWrite directly targets the pin instead of a channel
  ledcWrite(led1Pin, dutyCycle1);
  ledcWrite(led2Pin, dutyCycle2);

  // Fade LED 1
  dutyCycle1 += fadeAmount1;
  if (dutyCycle1 <= 0 || dutyCycle1 >= 255) {
    fadeAmount1 = -fadeAmount1;
  }

  // Fade LED 2
  dutyCycle2 += fadeAmount2;
  if (dutyCycle2 <= 0 || dutyCycle2 >= 255) {
    fadeAmount2 = -fadeAmount2;
  }

  delay(30);
}