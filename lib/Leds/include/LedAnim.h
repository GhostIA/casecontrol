// LedAnim.h — animation ENGINE. Reads the band/palette from Temp; owns only
// the motion (wave on strips, temporal gradient on fan) and transitions.
//   strips : moving wave (per-pixel phase)
//   fan    : uniform color cycling through the band palette over time
// Palette changes crossfade with easing. Gamma is handled in Leds.
// To change COLORS or band thresholds, edit the BANDS[] table in Temp.cpp.
#pragma once
#include <stdint.h>
#include "Temp.h"   // Band, tempBand, tempPalette, tempBandScale

// Motion tunables (not colors). Grab with ledAnimConfig(), edit live.
struct LedAnimSettings {
  float    waveSpeed;      // strip wave scroll (cycles/sec)
  float    waveSpread;     // palette repeats along a strip
  float    fanSpeed;       // fan color-cycle speed (cycles/sec)
  float    rainbowSpeed;   // rainbow hue scroll (cycles/sec)
  float    warnHz;         // warning pulse frequency
  uint32_t crossfadeMs;    // band transition blend time
};

void             ledAnimInit();
void             ledAnimTick(float tempC, bool valid);
LedAnimSettings &ledAnimConfig();