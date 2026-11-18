#include "LedAnim.h"
#include "Leds.h"
#include "Temp.h"
#include "config.h"
#include <Arduino.h>
#include <math.h>

// Motion tunables (colors + thresholds live in Temp.cpp's BANDS[] table).
static LedAnimSettings cfg = {
  /* waveSpeed    */ 0.6f,
  /* waveSpread   */ 3.0f,
  /* fanSpeed     */ 0.25f,
  /* rainbowSpeed */ 0.3f,
  /* warnHz       */ 1.5f,
  /* crossfadeMs  */ 1500,
};

static Band     curBand = B_RAINBOW;
static Band     prevBand = B_RAINBOW;
static uint32_t bandChangeAt = 0;
static uint32_t animStartMs  = 0;

LedAnimSettings &ledAnimConfig() { return cfg; }

// --- easing: smoothstep (ease-in-out). Swap here to change transition feel. ---
static float ease(float x) {
  if (x <= 0.0f) return 0.0f;
  if (x >= 1.0f) return 1.0f;
  return x * x * (3.0f - 2.0f * x);
}

// --- color helpers (gamma applied later in ledsColorRGB) ---
static RGB mul(const RGB &c, float s) {
  return { (uint8_t)(c.r * s), (uint8_t)(c.g * s), (uint8_t)(c.b * s) };
}
static RGB lerp(const RGB &a, const RGB &b, float f) {
  return { (uint8_t)(a.r + (b.r - a.r) * f),
           (uint8_t)(a.g + (b.g - a.g) * f),
           (uint8_t)(a.b + (b.b - a.b) * f) };
}
static RGB samplePalette(const Palette *p, float phase) {
  phase -= floorf(phase);
  float scaled = phase * p->count;
  int   i0 = (int)scaled;
  int   i1 = (i0 + 1) % p->count;
  return lerp(p->colors[i0], p->colors[i1], scaled - i0);
}
static RGB rainbow(float h) {
  h -= floorf(h);
  float r = fabsf(h * 6.0f - 3.0f) - 1.0f;
  float g = 2.0f - fabsf(h * 6.0f - 2.0f);
  float b = 2.0f - fabsf(h * 6.0f - 4.0f);
  auto cl = [](float x){ return x < 0 ? 0.0f : (x > 1 ? 1.0f : x); };
  return { (uint8_t)(cl(r)*255), (uint8_t)(cl(g)*255), (uint8_t)(cl(b)*255) };
}

static RGB stripPixel(Band b, uint16_t i, uint16_t len, float tSec) {
  if (b == B_WARNING) {
    float pulse = 0.5f + 0.5f * sinf(tSec * cfg.warnHz * TWO_PI);
    return { (uint8_t)(255 * pulse), 0, 0 };
  }
  float frac = (float)i / (float)len;
  if (b == B_RAINBOW) return rainbow(frac + tSec * cfg.rainbowSpeed);
  return samplePalette(tempPalette(b), frac * cfg.waveSpread + tSec * cfg.waveSpeed);
}

static RGB fanColor(Band b, float tSec) {
  if (b == B_WARNING) {
    float pulse = 0.5f + 0.5f * sinf(tSec * cfg.warnHz * TWO_PI);
    return { (uint8_t)(255 * pulse), 0, 0 };
  }
  if (b == B_RAINBOW) return rainbow(tSec * cfg.rainbowSpeed);
  return samplePalette(tempPalette(b), tSec * cfg.fanSpeed);
}

void ledAnimInit() {
  ledsBegin();
  animStartMs  = millis();
  bandChangeAt = animStartMs;
  curBand = prevBand = B_RAINBOW;
}

void ledAnimTick(float tempC, bool valid) {
  uint32_t now = millis();
  float tSec = (now - animStartMs) / 1000.0f;

  Band want = tempBand(tempC, valid);   // <-- band decided by Temp
  if (want != curBand) {
    prevBand     = curBand;
    curBand      = want;
    bandChangeAt = now;
  }

  float raw = 1.0f;
  if (now - bandChangeAt < cfg.crossfadeMs)
    raw = (float)(now - bandChangeAt) / (float)cfg.crossfadeMs;
  float blend = ease(raw);

  float curSc  = tempBandScale(curBand);    // <-- scale from Temp table
  float prevSc = tempBandScale(prevBand);

  for (uint16_t i = 0; i < STRIP_A_LEN; i++) {
    RGB c = mul(stripPixel(curBand, i, STRIP_A_LEN, tSec), curSc);
    if (blend < 1.0f)
      c = lerp(mul(stripPixel(prevBand, i, STRIP_A_LEN, tSec), prevSc), c, blend);
    ledsSetPixel(STRAND_STRIP_A, i, ledsColorRGB(c));
  }
  for (uint16_t i = 0; i < STRIP_B_LEN; i++) {
    RGB c = mul(stripPixel(curBand, i, STRIP_B_LEN, tSec), curSc);
    if (blend < 1.0f)
      c = lerp(mul(stripPixel(prevBand, i, STRIP_B_LEN, tSec), prevSc), c, blend);
    ledsSetPixel(STRAND_STRIP_B, i, ledsColorRGB(c));
  }

  RGB fc = mul(fanColor(curBand, tSec), curSc);
  if (blend < 1.0f)
    fc = lerp(mul(fanColor(prevBand, tSec), prevSc), fc, blend);
  ledsSetStrand(STRAND_FAN, FAN_LED_LEN, ledsColorRGB(fc));

  ledsShow();
}