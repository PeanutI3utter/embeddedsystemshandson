#include "LSM303D.hpp"

bool LSM303D::init() {
  uint8_t send_buffer[8];

  send_buffer[0] = 0x20 | AUTO_INCREMENT_BIT;
  send_buffer[1] = 0x57; // Enable acc
  send_buffer[2] = 0x00;
  send_buffer[3] = 0x00;
  send_buffer[4] = 0x00;
  send_buffer[5] = 0xF0;
  send_buffer[6] = 0x20; // +- 4 Gaus
  send_buffer[7] = 0x00; // Enable mag

  i2cAcquireBus(&I2CD1);
  i2cMasterTransmitTimeout(&I2CD1, SAD, send_buffer, 8, nullptr, 0, tmo);
  i2cReleaseBus(&I2CD1);

  light_sensor->init();

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

void LSM303D::readMagnetometer(uint8_t receiveBuffer[]) {
  readRegister(receiveBuffer, MAG_START);
};
