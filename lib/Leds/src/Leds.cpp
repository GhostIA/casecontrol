#include "Leds.h"
#include "config.h"
#include <Adafruit_NeoPXL8.h>
#include <math.h>

// Pin list comes from config.h (NEO_PIN[8]). NeoPXL8 wants an int8_t*, and its
// constructor copies the values, so a local non-const mirror is all it needs.
static int8_t neoPins[8] = {
  NEO_PIN[0], NEO_PIN[1], NEO_PIN[2], NEO_PIN[3],
  NEO_PIN[4], NEO_PIN[5], NEO_PIN[6], NEO_PIN[7],
};

static Adafruit_NeoPXL8 leds(STRAND_LEN, neoPins, NEO_GRB);

// --- gamma correction (applied at the single output chokepoint) ---
static float   gammaVal = 2.6f;
static uint8_t gammaLUT[256];

static void rebuildGamma() {
  for (int i = 0; i < 256; i++)
    gammaLUT[i] = (uint8_t)(powf(i / 255.0f, gammaVal) * 255.0f + 0.5f);
}

void ledsSetGamma(float g) { gammaVal = g; rebuildGamma(); }

// NeoPXL8 addresses pixels sequentially per strand: strand s pixel i -> s*N+i.
static inline uint16_t px(uint8_t strand, uint16_t i) {
  return (uint16_t)strand * STRAND_LEN + i;
}

void ledsBegin() {
  rebuildGamma();
  leds.begin();
  leds.setBrightness(LED_BRIGHTNESS);
}

uint32_t ledsColor(uint8_t r, uint8_t g, uint8_t b) {
  return leds.Color(gammaLUT[r], gammaLUT[g], gammaLUT[b]);
}

uint32_t ledsColorRGB(const RGB &c) {
  return leds.Color(gammaLUT[c.r], gammaLUT[c.g], gammaLUT[c.b]);
}

void ledsSetStrand(uint8_t strand, uint16_t len, uint32_t color) {
  for (uint16_t i = 0; i < len; i++) leds.setPixelColor(px(strand, i), color);
}

void ledsSetPixel(uint8_t strand, uint16_t i, uint32_t color) {
  leds.setPixelColor(px(strand, i), color);
}

void ledsShow() { leds.show(); }