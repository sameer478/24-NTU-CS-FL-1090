#include <Arduino.h>
// Muhammad Ayub
// 24 NTU CS F1 1059

#define LED_PIN 2

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("System Initialized - Heartbeat Mode Active");
}

void loop() {
  // First pulse
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Pulse 1: ON");
  delay(150);
  digitalWrite(LED_PIN, LOW);
  delay(150);

  // Second pulse (double blink)
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Pulse 2: ON");
  delay(150);
  digitalWrite(LED_PIN, LOW);

  // Pause before next heartbeat cycle
  Serial.println("Waiting for next cycle...");
  delay(1000);
}