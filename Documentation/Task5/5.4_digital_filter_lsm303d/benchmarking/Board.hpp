#include "utils.hpp"
#include <cstdint>

#pragma once
#define AUTO_INCREMENT_BIT (1 << 7)

class Board {
public:
  Board(uint8_t WHO_AM_I_VAL, uint8_t SAD) {
    this->WHO_AM_I_VAL = WHO_AM_I_VAL;
    this->SAD = SAD;
  };

  virtual bool init() = 0;
  virtual void readAccelerometer(uint8_t receive_buffer[]) = 0;

  bool whoAmICheck() {
    uint8_t send_buffer = 0x0F;
    uint8_t receive_buffer;

    i2cAcquireBus(&I2CD1);
    i2cMasterTransmitTimeout(&I2CD1, SAD, &send_buffer, 1, &receive_buffer, 1,
                             tmo);
    i2cReleaseBus(&I2CD1);
    if (receive_buffer != WHO_AM_I_VAL) {
      return false;
    }

    return true;
  }

protected:
  uint8_t SAD;
  uint8_t WHO_AM_I_VAL;
  const uint8_t WHO_AM_I = 0x0F;
};
