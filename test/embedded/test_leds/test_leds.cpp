// test_leds — on-target isolation test for NeoPXL8 strand mapping / wiring.
// Confirms strand 0/5/7 light on the correct physical outputs (GP16/21/23).
// Run: pio test -e scorpio -f embedded/test_leds
#include <Arduino.h>
#include <unity.h>
#include "config.h"
#include "Leds.h"

void test_leds_strand_mapping(void) {
  ledsBegin();
  // One strand at a time so you can verify which physical strip lights.
  ledsSetStrand(STRAND_STRIP_A, STRIP_A_LEN, ledsColor(0, 255, 0)); // A = green
  ledsShow(); delay(1000);
  ledsSetStrand(STRAND_STRIP_A, STRIP_A_LEN, 0);

  ledsSetStrand(STRAND_STRIP_B, STRIP_B_LEN, ledsColor(0, 0, 255)); // B = blue
  ledsShow(); delay(1000);
  ledsSetStrand(STRAND_STRIP_B, STRIP_B_LEN, 0);

  ledsSetStrand(STRAND_FAN, FAN_LED_LEN, ledsColor(255, 0, 0));     // fan = red
  ledsShow(); delay(1000);

  TEST_PASS_MESSAGE("Verify: strip A green, then strip B blue, then fan ring red.");
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_leds_strand_mapping);
  UNITY_END();
}

void loop() {}