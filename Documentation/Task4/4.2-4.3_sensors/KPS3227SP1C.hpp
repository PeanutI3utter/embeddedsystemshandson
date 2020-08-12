#ifndef KPS3227SP1C_H
#define KPS3227SP1C_H

#include "LED.hpp"
#include <hal.h>

class KPS3227SP1C {
public:
  const ADCConversionGroup *sensorADC;
  static const uint32_t sample_num = 50;
  uint16_t low = 0;
  uint16_t high = 0;
  adcsample_t value[sample_num];
  uint8_t state = 0;

  void init();
  short read();
  void stateTransition();
  adcsample_t readInt();

  KPS3227SP1C(LED *led1, LED *led2, ADCDriver *driver) {
    this->driver = driver;
    this->led1 = led1;
    this->led2 = led2;
  };

private:
  ADCDriver *driver;
  LED *led1;
  LED *led2;
};

#endif
