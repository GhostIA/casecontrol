// FanCurve.h — pure temperature->duty logic, no hardware dependency.
// A lib/ component so both src/main and test/test_fan_curve can #include it
// (PlatformIO Shared Code recommendation).
#pragma once
#include <stdint.h>

// Linear ramp: <=tMin -> floorDuty, >=tMax -> maxDuty, linear between.
// If tMax <= tMin (misconfig), fails safe to maxDuty.
uint8_t fanCurveDuty(float tempC, float tMin, float tMax,
                     uint8_t floorDuty, uint8_t maxDuty);