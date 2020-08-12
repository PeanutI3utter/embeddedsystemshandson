#include "LSM303D.hpp"

bool LSM303D::init() {
  uint8_t send_buffer[2];

  send_buffer[0] = 0x20 | AUTO_INCREMENT_BIT;
  send_buffer[1] = 0x57; // Enable acc 50 MHz

  i2cAcquireBus(&I2CD1);
  i2cMasterTransmitTimeout(&I2CD1, SAD, send_buffer, 2, nullptr, 0, tmo);
  i2cReleaseBus(&I2CD1);

  return whoAmICheck();
}

void LSM303D::readRegister(uint8_t receiveBuffer[], uint8_t regAdress) {
  uint8_t sendBuffer = regAdress | (1 << 7);

  i2cAcquireBus(&I2CD1);
  i2cMasterTransmitTimeout(&I2CD1, SAD, &sendBuffer, 1, receiveBuffer, 6, tmo);
  i2cReleaseBus(&I2CD1);
};

void LSM303D::readAccelerometer(uint8_t receiveBuffer[]) {
  readRegister(receiveBuffer, ACC_START);
};
