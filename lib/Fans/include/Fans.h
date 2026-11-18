// Fans.h — EMC2101 hardware only: read the ambient diode, apply a duty.
// The duty DECISION lives in Temp (tempDuty); this component just actuates it,
// so no curve/band logic here.
#pragma once
#include <stdint.h>

bool  fansBegin();                 // true if EMC2101 responded on Wire1
float fansReadTemp();              // external diode, deg C
bool  fansTempValid(float tempC);  // false if diode open/faulted
void  fansSetDuty(uint8_t dutyPct);// 0-100, applied to the EMC2101