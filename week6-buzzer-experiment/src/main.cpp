/*
 * Embedded IoT Systems - Homework 2
 * Dual-LED PWM fading (different frequencies) + SSD1306 OLED monitor
 *
 * Board : ESP32 DevKit (classic ESP32), Arduino-ESP32 core 2.0.x (LEDC API: ledcSetup/ledcAttachPin/ledcWrite)
 * LED1  : GPIO25, LEDC channel 0 (timer 0), 1000 Hz
 * LED2  : GPIO26, LEDC channel 2 (timer 1), 5000 Hz
 * OLED  : SSD1306 128x64 I2C @ 0x3C, SDA=GPIO21, SCL=GPIO22
 *
 * NOTE (technical check): on the classic ESP32, LEDC channels are paired onto
 * timers (channel/2). Channels 0 and 1 share timer 0, so they would be forced
 * to the SAME frequency. Using channel 0 and channel 2 puts the LEDs on
 * different timers, so each can have its own frequency.
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- Pins ----------
#define LED1_PIN   25
#define LED2_PIN   26
#define OLED_SDA   21
#define OLED_SCL   22

// ---------- OLED ----------
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  64
#define OLED_ADDR      0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- PWM configuration ----------
const uint8_t  LED1_CH   = 0;      // timer 0
const uint8_t  LED2_CH   = 2;      // timer 1 (different timer -> independent frequency)
const uint32_t LED1_FREQ = 1000;   // Hz
const uint32_t LED2_FREQ = 5000;   // Hz
const uint8_t  PWM_RES   = 10;     // 10-bit resolution -> duty 0..1023
const uint32_t DUTY_MAX  = (1UL << PWM_RES) - 1;   // 1023

// ---------- Fade state (these are the exact values written to hardware) ----------
int32_t duty1 = 0;
int32_t duty2 = DUTY_MAX;          // start LED2 at opposite end so the two differ
int32_t step1 = 8;                 // LED1 slower fade
int32_t step2 = -16;               // LED2 faster fade

const uint32_t FADE_INTERVAL_MS    = 15;
const uint32_t DISPLAY_INTERVAL_MS = 100;
uint32_t lastFade = 0, lastDisplay = 0;

static inline int dutyToPercent(int32_t duty) {
  return (int)((duty * 100L + DUTY_MAX / 2) / DUTY_MAX);   // rounded
}

// Move one triangular-wave step and clamp at the ends
void stepFade(int32_t &duty, int32_t &step) {
  duty += step;
  if (duty >= (int32_t)DUTY_MAX) { duty = DUTY_MAX; step = -step; }
  if (duty <= 0)                 { duty = 0;        step = -step; }
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(30, 0);
  display.println("PWM MONITOR");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  display.setCursor(0, 16);
  display.printf("LED1: %3d%% F:%luHz", dutyToPercent(duty1), (unsigned long)LED1_FREQ);
  display.setCursor(0, 28);
  display.printf("LED2: %3d%% F:%luHz", dutyToPercent(duty2), (unsigned long)LED2_FREQ);

  // Bar graphs (width proportional to the real duty values)
  display.drawRect(0, 42, 128, 8, SSD1306_WHITE);
  display.fillRect(0, 42, (duty1 * 128L) / DUTY_MAX, 8, SSD1306_WHITE);
  display.drawRect(0, 54, 128, 8, SSD1306_WHITE);
  display.fillRect(0, 54, (duty2 * 128L) / DUTY_MAX, 8, SSD1306_WHITE);

  display.display();
}

void setup() {
  Serial.begin(115200);

  // LEDC setup: channel, frequency, resolution
  ledcSetup(LED1_CH, LED1_FREQ, PWM_RES);
  ledcSetup(LED2_CH, LED2_FREQ, PWM_RES);
  ledcAttachPin(LED1_PIN, LED1_CH);
  ledcAttachPin(LED2_PIN, LED2_CH);

  // OLED setup
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 not found - check wiring/address");
    for (;;) delay(1000);
  }
  display.clearDisplay();
  display.display();

  Serial.printf("LED1: %lu Hz, LED2: %lu Hz, %u-bit resolution\n",
                (unsigned long)LED1_FREQ, (unsigned long)LED2_FREQ, PWM_RES);
}

void loop() {
  uint32_t now = millis();

  if (now - lastFade >= FADE_INTERVAL_MS) {
    lastFade = now;
    stepFade(duty1, step1);
    stepFade(duty2, step2);
    ledcWrite(LED1_CH, duty1);
    ledcWrite(LED2_CH, duty2);
  }

  if (now - lastDisplay >= DISPLAY_INTERVAL_MS) {
    lastDisplay = now;
    updateDisplay();
    Serial.printf("LED1=%3d%%  LED2=%3d%%\n", dutyToPercent(duty1), dutyToPercent(duty2));
  }
}