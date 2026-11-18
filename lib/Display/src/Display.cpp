#include "Display.h"
#include "config.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <math.h>

// Pass SPI1 explicitly — on the SCORPIO the panel is wired to the SPI1 bus
// (GP14/15). Handing the default SPI here is the classic white-screen bug.
static Adafruit_GC9A01A tft(&SPI1, TFT_DC, TFT_CS, TFT_RST);

static const uint16_t COL_BG   = GC9A01A_BLACK;
static const uint16_t COL_TEMP = GC9A01A_WHITE;
static const uint16_t COL_RING = 0x2945;         // dim slate track
static const uint16_t COL_FILL = GC9A01A_CYAN;   // duty arc

static int   lastDuty = -1;
static float lastTemp = -999.0f;

// Thick arc from a0..a1 degrees, 0 = top, clockwise.
static void arc(int cx, int cy, int rOut, int rIn,
                float a0, float a1, uint16_t color) {
  for (float a = a0; a <= a1; a += 1.0f) {
    float rad = (a - 90.0f) * DEG_TO_RAD;
    float c = cosf(rad), s = sinf(rad);
    for (int r = rIn; r <= rOut; r++)
      tft.drawPixel((int)(cx + c * r), (int)(cy + s * r), color);
  }
}

void displayBegin() {
  SPI1.setSCK(TFT_SCK);
  SPI1.setTX(TFT_MOSI);           // write-only panel; no MISO
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(COL_BG);
  arc(120, 120, 116, 104, 0, 360, COL_RING);
  lastDuty = -1;
  lastTemp = -999.0f;
}

void displayUpdate(float tempC, uint8_t duty, bool valid) {
  if ((int)duty != lastDuty) {
    arc(120, 120, 116, 104, 0, 360, COL_RING);              // clear track
    if (duty > 0) arc(120, 120, 116, 104, 0, 3.6f * duty, COL_FILL);
    lastDuty = duty;
  }

  if (fabsf(tempC - lastTemp) >= 0.1f || !valid) {
    tft.fillRect(40, 92, 160, 56, COL_BG);
    tft.setTextColor(valid ? COL_TEMP : GC9A01A_RED);
    tft.setTextSize(6);
    char buf[8];
    if (valid) snprintf(buf, sizeof(buf), "%2.0f", (double)tempC);
    else       snprintf(buf, sizeof(buf), "--");
    int w = strlen(buf) * 36;   // ~36px/char at size 6, approximate centering
    tft.setCursor(120 - w / 2, 96);
    tft.print(buf);
    tft.setTextSize(2);
    tft.setTextColor(COL_TEMP);
    tft.setCursor(120 + w / 2 + 4, 96);
    tft.print("C");
    lastTemp = tempC;
  }

  tft.fillRect(70, 156, 100, 20, COL_BG);
  tft.setTextSize(2);
  tft.setTextColor(COL_FILL);
  char db[12];
  snprintf(db, sizeof(db), "FAN %u%%", duty);
  tft.setCursor(120 - (int)(strlen(db) * 6), 158);
  tft.print(db);
}