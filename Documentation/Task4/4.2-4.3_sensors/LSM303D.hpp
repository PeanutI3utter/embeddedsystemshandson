#include "Board.hpp"

class LSM303D : public Board {
public:
  LSM303D(KPS3227SP1C *light_sensor)
      : Board((uint8_t)0x49, (uint8_t)0x1D, light_sensor){};

  virtual bool init();
  virtual void readAccelerometer(uint8_t receive_buffer[]);
  virtual void readMagnetometer(uint8_t receive_buffer[]);

private:
  uint8_t ACC_START = 0x28;
  uint8_t MAG_START = 0x08;
  void readRegister(uint8_t receiveBuffer[], uint8_t regAdress);
};
