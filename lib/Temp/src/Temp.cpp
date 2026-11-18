#include "Temp.h"
#include "config.h"
#include "FanCurve.h"

// ===========================================================================
// THE BAND TABLE — single edit point for colors AND temperature thresholds.
//   - recolor a band: edit its COLORS_* array
//   - move a boundary: edit a row's maxTemp
//   - change per-band brightness: edit a row's scale
// The fan duty ramp is separate (tempDuty, below) and uses config constants.
// ===========================================================================

static const RGB COLORS_COLD[] = {  // 15-25C
  {  0,   0, 139},   // dark blue
  {  0, 191, 255},   // light blue
  {255, 255, 255},   // white
};
static const RGB COLORS_COOL[] = {  // 25-35C
  {  0,   0, 255},   // blue
  {148,   0, 211},   // purple
  {  0, 255,   0},   // green
};
static const RGB COLORS_WARM[] = {  // 35-45C
  {255, 105, 180},   // pink
  {255, 140,   0},   // orange
  {255, 255,   0},   // yellow
};
static const RGB COLORS_HOT[] = {   // 45-55C
  {255,   0,   0},   // red
  {255, 140,   0},   // orange
  {255, 255,   0},   // yellow
};

static const Palette PAL_COLD = PALETTE(COLORS_COLD);
static const Palette PAL_COOL = PALETTE(COLORS_COOL);
static const Palette PAL_WARM = PALETTE(COLORS_WARM);
static const Palette PAL_HOT  = PALETTE(COLORS_HOT);

struct BandDef {
  float          maxTemp;   // upper bound, exclusive
  Band           id;
  const Palette *palette;   // nullptr = generated (rainbow / warning)
  float          scale;     // brightness 0..1
};

// Rows in ascending temperature order. First row's palette is null (rainbow),
// last row is the warning band above the top threshold.
static const BandDef BANDS[] = {
  { 15.0f, B_RAINBOW, nullptr,   1.0f },   // < 15C  -> rainbow
  { 25.0f, B_COLD,    &PAL_COLD, 0.6f },   // 15-25  -> dimmed (white-heavy)
  { 35.0f, B_COOL,    &PAL_COOL, 1.0f },   // 25-35
  { 45.0f, B_WARM,    &PAL_WARM, 1.0f },   // 35-45
  { 55.0f, B_HOT,     &PAL_HOT,  1.0f },   // 45-55
  { 1e9f,  B_WARNING, nullptr,   1.0f },   // > 55C  -> warning
};
static const int NBANDS = sizeof(BANDS) / sizeof(BANDS[0]);
// ===========================================================================

static const BandDef &defFor(Band b) {
  for (int i = 0; i < NBANDS; i++) if (BANDS[i].id == b) return BANDS[i];
  return BANDS[0];
}

Band tempBand(float t, bool valid) {
  if (!valid) return B_RAINBOW;
  for (int i = 0; i < NBANDS; i++) if (t < BANDS[i].maxTemp) return BANDS[i].id;
  return B_WARNING;
}

const Palette *tempPalette(Band b) { return defFor(b).palette; }
float          tempBandScale(Band b) { return defFor(b).scale; }

// --- fan duty: piecewise curve (default = the config 2-point linear ramp) ---
// Add points here to shape the curve (e.g. a quiet mid-step). Kept in sync with
// the config constants so the default behavior is unchanged.
static const FanCurve::Point CURVE_POINTS[] = {
  { TEMP_MIN_C, DUTY_FLOOR },   // 28C -> 30%
  { TEMP_MAX_C, DUTY_MAX   },   // 45C -> 100%
};
static const FanCurve fanCurve = FAN_CURVE(CURVE_POINTS);

uint8_t tempDuty(float t, bool valid) {
  if (!valid) return DUTY_MAX;      // sensor fault -> full cooling
  return fanCurve.duty(t);
}