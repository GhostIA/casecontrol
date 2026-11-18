// test_temp — native (host) unit test for the Temp band model: which band a
// temperature maps to, and which bands carry palettes. FanCurve has its own
// test (test_fan_curve). No hardware.
//   pio test -e native -f test_temp
#include <unity.h>
#include <Temp.h>
#include "config.h"   // DUTY_MAX

// --- band selection ---
void test_band_boundaries(void) {
  TEST_ASSERT_EQUAL(B_RAINBOW, tempBand(10.0f, true));
  TEST_ASSERT_EQUAL(B_COLD,    tempBand(20.0f, true));
  TEST_ASSERT_EQUAL(B_COOL,    tempBand(30.0f, true));
  TEST_ASSERT_EQUAL(B_WARM,    tempBand(40.0f, true));
  TEST_ASSERT_EQUAL(B_HOT,     tempBand(50.0f, true));
  TEST_ASSERT_EQUAL(B_WARNING, tempBand(60.0f, true));
}
void test_band_edges_are_exclusive_upper(void) {
  // exactly 25.0 belongs to the next band up (COOL), not COLD
  TEST_ASSERT_EQUAL(B_COOL, tempBand(25.0f, true));
  TEST_ASSERT_EQUAL(B_HOT,  tempBand(45.0f, true));
}
void test_invalid_is_rainbow(void) {
  TEST_ASSERT_EQUAL(B_RAINBOW, tempBand(30.0f, false));
}

// --- palette presence: named bands have palettes, generated ones don't ---
void test_palette_presence(void) {
  TEST_ASSERT_NOT_NULL(tempPalette(B_COOL));
  TEST_ASSERT_NULL(tempPalette(B_RAINBOW));
  TEST_ASSERT_NULL(tempPalette(B_WARNING));
}

// --- tempDuty fail-safe on invalid sensor ---
void test_tempduty_invalid_is_full(void) {
  TEST_ASSERT_EQUAL_UINT8(DUTY_MAX, tempDuty(30.0f, false));
}

int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(test_band_boundaries);
  RUN_TEST(test_band_edges_are_exclusive_upper);
  RUN_TEST(test_invalid_is_rainbow);
  RUN_TEST(test_palette_presence);
  RUN_TEST(test_tempduty_invalid_is_full);
  return UNITY_END();
}