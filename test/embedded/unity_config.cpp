// unity_config.cpp — routes Unity output to the SCORPIO USB serial.
// Shared across all test/embedded/* suites.
#include <Arduino.h>
#include "unity_config.h"

extern "C" {

void unityOutputStart(unsigned long baud) {
  Serial.begin(baud);
  // RP2040 native USB: wait briefly so the host can attach before output.
  delay(2000);
}

void unityOutputChar(unsigned int c) { Serial.write((char)c); }
void unityOutputFlush(void)          { Serial.flush(); }
void unityOutputComplete(void)       { Serial.end(); }

}