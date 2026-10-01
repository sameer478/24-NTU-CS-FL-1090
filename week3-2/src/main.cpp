// Week3-Lecture2
// Timer Interrupt (Internal) - Updated for Arduino Core 2.x
// Embedded IoT System Fall-2026

#include <Arduino.h>

#define LED 4

hw_timer_t *My_timer = NULL;

void IRAM_ATTR onTimer() {             // IRAM_ATTR is the standard for Core 2.x
  digitalWrite(LED, !digitalRead(LED));     
}

void setup() {
  pinMode(LED, OUTPUT);

  // 1. timerBegin(timer_id, prescaler, countUp)
  // ESP32 base clock is 80MHz. Prescaler of 80 = 1MHz (1 tick = 1 µs).
  My_timer = timerBegin(0, 80, true); 

  // 2. timerAttachInterrupt(timer, ISR, edge)
  // edge=true means trigger on the rising edge
  timerAttachInterrupt(My_timer, &onTimer, true);

  // 3. timerAlarmWrite(timer, tick_count, autoreload)
  // 1,000,000 ticks = 1 second
  timerAlarmWrite(My_timer, 1000000, true);
  
  // 4. You must explicitly enable the alarm in v2.x
  timerAlarmEnable(My_timer);
}

void loop() {
  // nothing needed, all handled by interrupts
}