// test_fan_curve — native (host) unit test for the FanCurve struct.
// Pure piecewise-linear temp->duty; no hardware.
//   pio test -e native -f test_fan_curve
#include <unity.h>
#include <FanCurve.h>
#include <Temp.h>

// Default project curve: 28C 30% -> 45C 100% (2-point linear ramp).
static const FanCurve::Point PTS[] = { {28.0f, 30}, {45.0f, 100} };
static const FanCurve CURVE = FAN_CURVE(PTS);

void test_below_first_is_floor(void) {
  TEST_ASSERT_EQUAL_UINT8(30, CURVE.duty(20.0f));
  TEST_ASSERT_EQUAL_UINT8(30, CURVE.duty(28.0f));   // exactly the first point
}

void test_above_last_is_max(void) {
  TEST_ASSERT_EQUAL_UINT8(100, CURVE.duty(45.0f));  // exactly the last point
  TEST_ASSERT_EQUAL_UINT8(100, CURVE.duty(60.0f));
}

void test_midpoint(void) {
  // 36.5C is halfway across 28..45 -> ~65% (halfway 30..100).
  TEST_ASSERT_UINT8_WITHIN(1, 65, CURVE.duty(36.5f));
}

void test_monotonic(void) {
  uint8_t prev = 0;
  for (float t = 20.0f; t <= 50.0f; t += 0.5f) {
    uint8_t d = CURVE.duty(t);
    TEST_ASSERT_TRUE(d >= prev);   // never decreases as temp rises
    prev = d;
  }
}

void test_piecewise_three_points(void) {
  // Quiet-flat low, aggressive high: {20:20, 40:40, 50:100}.
  static const FanCurve::Point P3[] = { {20.0f, 20}, {40.0f, 40}, {50.0f, 100} };
  FanCurve c = FAN_CURVE(P3);
  TEST_ASSERT_EQUAL_UINT8(20,  c.duty(20.0f));   // first point
  TEST_ASSERT_EQUAL_UINT8(30,  c.duty(30.0f));   // mid of gentle segment
  TEST_ASSERT_EQUAL_UINT8(40,  c.duty(40.0f));   // the knee
  TEST_ASSERT_EQUAL_UINT8(70,  c.duty(45.0f));   // mid of steep segment
  TEST_ASSERT_EQUAL_UINT8(100, c.duty(50.0f));   // last point
}

void test_degenerate_curves(void) {
  FanCurve empty = { nullptr, 0 };
  TEST_ASSERT_EQUAL_UINT8(0, empty.duty(30.0f));           // no points -> 0

  static const FanCurve::Point ONE[] = { {30.0f, 55} };
  FanCurve single = FAN_CURVE(ONE);
  TEST_ASSERT_EQUAL_UINT8(55, single.duty(10.0f));         // single point -> const
  TEST_ASSERT_EQUAL_UINT8(55, single.duty(99.0f));
}

void test_duplicate_temp_guard(void) {
  // Two points at the same temp must not divide by zero.
  static const FanCurve::Point DUP[] = { {30.0f, 40}, {30.0f, 80}, {40.0f, 100} };
  FanCurve c = FAN_CURVE(DUP);
  uint8_t d = c.duty(30.0f);
  TEST_ASSERT_TRUE(d == 40 || d == 80);   // deterministic, not NaN/garbage
}

int main(int, char **) {
  UNITY_BEGIN();
  RUN_TEST(test_below_first_is_floor);
  RUN_TEST(test_above_last_is_max);
  RUN_TEST(test_midpoint);
  RUN_TEST(test_monotonic);
  RUN_TEST(test_piecewise_three_points);
  RUN_TEST(test_degenerate_curves);
  RUN_TEST(test_duplicate_temp_guard);
  return UNITY_END();
}