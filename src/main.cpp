// main.cpp — production entry. Temp controls BOTH the fan (duty) and the
// LED animation (band/palette); this file just wires the pieces together.
//
// WHERE TO CHANGE THINGS:
//   colors + band thresholds ...... lib/Temp/src/Temp.cpp  (BANDS[] table)
//   fan ramp (28->45, 30->100%) ... include/config.h       (TEMP_*, DUTY_*)
//   pins ......................... include/config.h
//   animation feel (speeds etc) .. ledAnimConfig() below
//   gamma ........................ ledsSetGamma() below
#include <Arduino.h>
#include "config.h"
#include "Temp.h"
#include "LedAnim.h"
#include "Fans.h"
#include "Display.h"
#include <Leds.h>

void setup() {
  Serial.begin(115200);

  ledAnimInit();               // starts on rainbow until a temp reading
  ledsSetGamma(2.6f);          // smoother fades; 1.0 = off

  LedAnimSettings &a = ledAnimConfig();
  a.waveSpeed   = 0.6f;
  a.fanSpeed    = 0.25f;
  a.crossfadeMs = 1500;

  displayBegin();

  if (!fansBegin())
    Serial.println("EMC2101 not found on Wire1 (GP2/GP3)!");
}

void loop() {
  static float   t     = 0.0f;
  static bool    valid = false;
  static uint8_t duty  = DUTY_FLOOR;

  // Temp/fan/display need only ~2 Hz; the animation runs every loop.
  static uint32_t lastSlow = 0;
  uint32_t now = millis();
  if (now - lastSlow >= 500) {
    lastSlow = now;
    t     = fansReadTemp();
    valid = fansTempValid(t);
    duty  = tempDuty(t, valid);   // Temp decides duty
    fansSetDuty(duty);            // Fans just applies it
    displayUpdate(t, duty, valid);
  }

  ledAnimTick(t, valid);          // LedAnim asks Temp for the band internally
}