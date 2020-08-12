#include "KPS3227SP1C.hpp"

void KPS3227SP1C::stateTransition() {
  switch (state) {
  case 0: // normal state, user can read
    state = 1;
    led1->on();
    break;
  case 1: // calibrate low(dark) value
    state = 2;
    led1->off();
    led2->on();
    low = value[sample_num - 1];
    break;
  case 2: // calibrate high(bright) value
    state = 0;
    led2->off();
    high = value[sample_num - 1];
    break;
  default: // undefined state, should not be reached in any way
    led1->on();
    led2->on();
    state = 3;
  }
}

short KPS3227SP1C::read() {
  short val;

  switch (state) {
  case 0: // normal state, return percentage of max illuminance
    val = (value[sample_num - 1] - low) * 100 / (high - low);
    return val > 100 ? 100 : val < 0 ? 0 : val;
  default:
    // calibration mode
    return -1;
  }
}

adcsample_t KPS3227SP1C::readInt() { return value[sample_num - 1]; }

void onBufferFullDummy(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
  (void)adcp;
  (void)buffer;
  (void)n;
}

void KPS3227SP1C::init() {
  adcStartConversion(driver, sensorADC, value, sample_num);
  stateTransition();
}
