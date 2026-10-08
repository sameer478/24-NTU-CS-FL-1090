#include <Arduino.h>

const int BUTTON_PIN = 4;   // button between GPIO4 and GND
const int LED_PIN    = 2;   // onboard LED
const unsigned long DEBOUNCE_MS = 50;

int lastReading = HIGH;      // last raw reading
int stableState = HIGH;      // debounced state
unsigned long lastChange = 0;
bool ledOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  // Raw value changed (could be bounce) -> restart timer
  if (reading != lastReading) {
    lastChange = millis();
    lastReading = reading;
  }

  // Value stayed the same long enough -> accept it
  if ((millis() - lastChange) > DEBOUNCE_MS && reading != stableState) {
    stableState = reading;

    if (stableState == LOW) {          // button pressed
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn);
      Serial.println("Button pressed");
    }
  }
}