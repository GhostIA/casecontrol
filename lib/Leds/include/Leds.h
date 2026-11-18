// Leds.h — NeoPXL8 wrapper. Owns strand->index math AND the single output
// chokepoint where gamma is applied to every color.
#pragma once
#include <stdint.h>
#include "Palette.h"   // RGB (now in root include/)

void     ledsBegin();

// Gamma applied inside ledsColor()/ledsColorRGB() so ALL output is corrected
// in one place. 1.0 = off; 2.2-2.8 typical. Default 2.6.
void     ledsSetGamma(float g);

uint32_t ledsColor(uint8_t r, uint8_t g, uint8_t b);   // gamma-corrected
uint32_t ledsColorRGB(const RGB &c);                   // gamma-corrected
void     ledsSetStrand(uint8_t strand, uint16_t len, uint32_t color);
void     ledsSetPixel(uint8_t strand, uint16_t i, uint32_t color);
void     ledsShow();