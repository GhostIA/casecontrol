// Palette.h — hardware-free color + palette TYPES only (no driver deps).
// Kept separate from Leds.h so config.h (and thus the native Temp build)
// can reference palettes without pulling in NeoPXL8.
#pragma once
#include <stdint.h>
 
struct RGB { uint8_t r, g, b; };
 
struct Palette {
  const RGB *colors;
  uint8_t    count;
};
 
// Count array length automatically.
#define PALETTE(arr) { (arr), (uint8_t)(sizeof(arr) / sizeof((arr)[0])) }
 