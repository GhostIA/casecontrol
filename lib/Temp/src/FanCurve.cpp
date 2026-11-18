#include "FanCurve.h"

uint8_t FanCurve::duty(float tempC) const {
  if (count == 0) return 0;
  if (count == 1) return points[0].duty;

  // Below the first point -> floor; at/above the last -> max.
  if (tempC <= points[0].tempC)          return points[0].duty;
  if (tempC >= points[count - 1].tempC)  return points[count - 1].duty;

  // Find the segment [i, i+1] containing tempC and interpolate.
  for (uint8_t i = 0; i < count - 1; i++) {
    const Point &a = points[i];
    const Point &b = points[i + 1];
    if (tempC >= a.tempC && tempC <= b.tempC) {
      if (b.tempC == a.tempC) return a.duty;   // guard duplicate temps
      float frac = (tempC - a.tempC) / (b.tempC - a.tempC);
      return (uint8_t)(a.duty + frac * ((float)b.duty - a.duty) + 0.5f);
    }
  }
  return points[count - 1].duty;   // unreachable, defensive
}