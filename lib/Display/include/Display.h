// Display.h — GC9A01 round TFT UI: ambient temp hero number + fan-duty ring.
#pragma once
#include <stdint.h>

void displayBegin();
void displayUpdate(float tempC, uint8_t duty, bool valid);