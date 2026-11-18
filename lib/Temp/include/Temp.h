// Temp.h — the temperature CONTROLLER. Single source of truth for what a
// temperature *means*: which band, which palette, what fan duty.
// Pure (no hardware) so it stays host-testable. Both Fans (duty) and LedAnim
// (palette) read from here, so bands are defined exactly once.
#pragma once
#include <stdint.h>
#include "Palette.h"

// Band identity. RAINBOW (<15C / invalid) and WARNING (>55C) are generated,
// not palette-based.
enum Band { B_RAINBOW, B_COLD, B_COOL, B_WARM, B_HOT, B_WARNING };

// --- band model (edit the BANDS[] table in Temp.cpp to change any of this) ---
Band           tempBand(float tempC, bool valid);   // temp -> band
const Palette *tempPalette(Band b);                 // band -> colors (or null)
float          tempBandScale(Band b);               // band -> brightness 0..1

// --- fan duty ---
// The curve lives in FanCurve.h/.cpp (piecewise, testable). tempDuty applies
// the default curve + a sensor-fault fail-safe.
uint8_t tempDuty(float tempC, bool valid);