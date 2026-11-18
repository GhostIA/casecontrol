#include "Fans.h"
#include "config.h"
#include <Wire.h>
#include <Adafruit_EMC2101.h>

static Adafruit_EMC2101 emc;
static bool emcOK = false;

bool fansBegin() {
  // SCORPIO STEMMA QT is GP2/GP3 = I2C1 -> Wire1 (must set pins on board=pico).
  Wire1.setSDA(I2C_SDA);
  Wire1.setSCL(I2C_SCL);
  Wire1.begin();
  emcOK = emc.begin(EMC2101_I2CADDR_DEFAULT, &Wire1);
  if (emcOK) {
    emc.LUTEnabled(false);        // manual duty, not the chip's internal LUT
    emc.setDutyCycle(DUTY_FLOOR);
    // If the Noctuas whine/misbehave: emc.setPWMFrequency(...); emc.setPWMDivisor(...);
  }
  return emcOK;
}

float fansReadTemp() {
  return emcOK ? emc.getExternalTemperature() : -100.0f;
}

bool fansTempValid(float tempC) {
  return (tempC > -60.0f && tempC < 150.0f);   // EMC2101 open-diode sentinel
}

void fansSetDuty(uint8_t dutyPct) {
  if (emcOK) emc.setDutyCycle(dutyPct);
}