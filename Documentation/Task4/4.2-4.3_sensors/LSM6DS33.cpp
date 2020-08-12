#include "LSM6DS33.hpp"

bool LSM6DS33::init() {
  uint8_t send_buffer[2];

  send_buffer[0] = 0x10;
  send_buffer[1] = 0x50;

  i2cAcquireBus(&I2CD1);
  i2cMasterTransmitTimeout(&I2CD1, SAD, send_buffer, 2, nullptr, 0, tmo);
  i2cReleaseBus(&I2CD1);

  send_buffer[0] = 0x22;
  send_buffer[1] = 0x0;

  i2cAcquireBus(&I2CD1);
  i2cMasterTransmitTimeout(&I2CD1, 0x1E, send_buffer, 2, nullptr, 0, tmo);
  i2cReleaseBus(&I2CD1);

  light_sensor->init();

  return whoAmICheck();
}

void LSM6DS33::readAccelerometer(uint8_t receive_buffer[]) {
  uint8_t send_buffer = ACC_START;

  i2cAcquireBus(&I2CD1);
  i2cMasterTransmitTimeout(&I2CD1, SAD, &send_buffer, 1, receive_buffer, 6,
                           tmo);
  i2cReleaseBus(&I2CD1);
};

void LSM6DS33::readMagnetometer(uint8_t receive_buffer[]) {

  uint8_t sendBuffer = 0x28 | (1 << 7);

  i2cAcquireBus(&I2CD1);
  i2cMasterTransmitTimeout(&I2CD1, 0x1E, &sendBuffer, 1, receive_buffer, 6,
                           tmo);
  i2cReleaseBus(&I2CD1);
};