// config.h — project-wide pins and tunables.
// Lives in include/ (root include dir), per the PlatformIO Pizza Project layout.
// Included by src/ and by lib/ components.
#pragma once
#include <stdint.h>

// ---- LED strands ----
static const uint16_t STRAND_LEN   = 80;   // longest strand -> NeoPXL8 RAM size
static const uint16_t STRIP_A_LEN  = 80;   // strand 0  (GP16)
static const uint16_t STRIP_B_LEN  = 80;   // strand 7  (GP23)
static const uint16_t FAN_LED_LEN  = 8;    // strand 5  (GP21) TL-C12C-S = 8 LEDs

static const uint8_t  STRAND_STRIP_A = 0;  // -> GP16
static const uint8_t  STRAND_FAN     = 5;  // -> GP21
static const uint8_t  STRAND_STRIP_B = 7;  // -> GP23

// NeoPXL8 output pins, one per strand (index = strand number).
// SCORPIO's 8 level-shifted outputs are GP16..GP23; only 0/5/7 are wired.
// Leds.cpp assembles these into the int8_t[8] array NeoPXL8 wants.
static const int8_t NEO_PIN[8] = { 16, 17, 18, 19, 20, 21, 22, 23 };

static const uint8_t  LED_BRIGHTNESS = 90; // 0-255 global cap (heat/current)

// ---- Fan curve (linear ramp) ----
static const float   TEMP_MIN_C = 28.0f;   // at/below -> floor duty
static const float   TEMP_MAX_C = 45.0f;   // at/above -> 100%
static const uint8_t DUTY_FLOOR = 30;      // % (below ~20-25% a 40mm can stall)
static const uint8_t DUTY_MAX   = 100;     // %

// ---- GC9A01 on SPI1 ----
static const int8_t TFT_SCK  = 14;
static const int8_t TFT_MOSI = 15;         // write-only panel; no MISO
static const int8_t TFT_CS   = 5;
static const int8_t TFT_DC   = 6;
static const int8_t TFT_RST  = 9;

// ---- EMC2101 I2C: SCORPIO STEMMA QT = GP2/GP3 = I2C1 -> Wire1 ----
static const int8_t I2C_SDA = 2;
static const int8_t I2C_SCL = 3;