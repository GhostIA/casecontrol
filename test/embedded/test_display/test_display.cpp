// test_display — on-target isolation test for the GC9A01 (SPI1 wiring / white-
// screen debug). Not a pass/fail assertion of pixels (can't read them back);
// it drives the Display component and confirms begin() + a visible update.
// Run: pio test -e scorpio -f embedded/test_display
#include <Arduino.h>
#include <unity.h>
#include "Display.h"

void test_display_begin_and_draw(void) {
  displayBegin();
  // Sweep duty so you SEE the ring animate + temp number change on the panel.
  for (uint8_t d = 0; d <= 100; d += 20) {
    displayUpdate(20.0f + d * 0.25f, d, true);
    delay(300);
  }
  displayUpdate(0.0f, 100, false);   // fault state (RED "--") render check
  TEST_PASS_MESSAGE("Check the panel: ring swept 0->100%, then RED fault dashes.");
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_display_begin_and_draw);
}

void tearDown(void) {
    UNITY_END();
    // This runs AFTER every single test case
    // Add cleanup code here if needed
}


void loop() {}