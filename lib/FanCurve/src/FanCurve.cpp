#include "FanCurve.h"

uint8_t fanCurveDuty(float tempC, float tMin, float tMax,
                     uint8_t floorDuty, uint8_t maxDuty) {
  if (tMax <= tMin)  return maxDuty;      // misconfigured range -> fail safe
  if (tempC <= tMin) return floorDuty;
  if (tempC >= tMax) return maxDuty;
  float frac = (tempC - tMin) / (tMax - tMin);
  return (uint8_t)(floorDuty + frac * (maxDuty - floorDuty) + 0.5f);
}