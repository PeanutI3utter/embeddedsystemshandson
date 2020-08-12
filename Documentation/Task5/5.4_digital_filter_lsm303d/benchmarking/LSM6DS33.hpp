#include "Board.hpp"

class LSM6DS33 : public Board {
public:
  LSM6DS33() : Board((uint8_t)0x69, (uint8_t)0x6B){};

  virtual bool init();
  virtual void readAccelerometer(uint8_t receive_buffer[]);

private:
  uint8_t ACC_START = 0x28;
  uint8_t MAG_START = 0x08;
};
