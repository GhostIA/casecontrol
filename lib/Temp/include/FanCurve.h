// FanCurve.h — piecewise-linear temperature -> duty curve.
// Pure (no hardware): a table of {temp, duty} points, linearly interpolated.
// The current 2-point ramp is just the simplest case; add points to shape it
// (e.g. quiet-flat low, aggressive near the top). Lives in the Temp component.
#pragma once
#include <stdint.h>

struct FanCurve {
  struct Point { float tempC; uint8_t duty; };

  const Point *points;   // ascending by tempC
  uint8_t      count;

  // Duty for a temperature: clamps below first / above last point,
  // linearly interpolates between. Safe on count==0 (returns 0) and count==1.
  uint8_t duty(float tempC) const;
};

// Helper so callers never hand-count the point array length.
#define FAN_CURVE(arr) { (arr), (uint8_t)(sizeof(arr) / sizeof((arr)[0])) }