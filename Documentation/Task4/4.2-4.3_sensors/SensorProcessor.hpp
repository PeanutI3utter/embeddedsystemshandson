#include "Board.hpp"
#include <cstring>

typedef struct {
  float x;
  float y;
  float z;
} axis;

class SensorProcessor {
private:
  Board *board;

  // Divide read bits and calculate conversion factor
  const int intervall_size = 32768;
  // +-2 g: ~0.061 mg/LSB
  const double conversion_factor_acc = 2.0 / intervall_size;
  // +-4 Gauss: ~0.080 mGauss/LSB
  const double conversion_factor_mag = 4.0 / intervall_size;

  /**
   * Convert read data into float values for each axis
   */
  void convertData(uint8_t data[], axis *axis_data, double conversion_factor,
                   float constant) {
    const short x = (short)data[0] + (((short)data[1]) << 8);
    const short y = (short)data[2] + (((short)data[3]) << 8);
    const short z = (short)data[4] + (((short)data[5]) << 8);

    axis_data->x = x * conversion_factor * constant;
    axis_data->y = y * conversion_factor * constant;
    axis_data->z = z * conversion_factor * constant;
  };

  /**
   * Generate string from float with 2 decimal places after comma
   */
  void ftoa(float value, char str[]) {
    int start = 0;

    if (value < 0) {
      str[0] = '-';
      value = value * -1;
      start = 1;
    }

    int first_digit = 0;
    if (value >= 10) {
      first_digit = (int)(value / 10);
      value = value - ((int)(value / 10) * 10);
    }

    if (first_digit != 0) {
      str[start++] = '0' + first_digit;
    }

    str[start++] = '0' + (int)value;
    str[start++] = '.';
    value = (value - (int)value) * 10;
    str[start++] = '0' + (int)value;
    value = (value - (int)value) * 10;
    str[start++] = '0' + (int)value;
    str[start++] = '\0';
  }

  /**
   * Generate human readable String
   */
  void generateResponseString(axis *axis_data, char msg[]) {
    char x[7];
    ftoa(axis_data->x, x);

    char y[7];
    ftoa(axis_data->y, y);

    char z[7];
    ftoa(axis_data->z, z);

    strcpy(msg, "X: ");
    strcat(msg, x);
    strcat(msg, ", Y: ");
    strcat(msg, y);
    strcat(msg, ", Z: ");
    strcat(msg, z);
  };

public:
  SensorProcessor(Board *board) {
    this->board = board;
    while (!board->init()) {
      // Fallback Handler if WHO_AM_I_VALUE is not correct or device is not
      // connected -> wait for device
    }
  };

  void readAccelerometer(char msg[]) {
    uint8_t data[6];
    board->readAccelerometer(data);

    axis axis_data;
    convertData(data, &axis_data, conversion_factor_acc, 9.81);
    generateResponseString(&axis_data, msg);
  }

  void readMagnetometer(char msg[]) {
    uint8_t data[6];
    board->readMagnetometer(data);

    axis axis_data;
    convertData(data, &axis_data, conversion_factor_mag, 1);
    generateResponseString(&axis_data, msg);
  }

  bool readLightSensor(char msg[]) {
    const uint8_t ascii_nums[] = {0x30, 0x31, 0x32, 0x33, 0x34,
                                  0x35, 0x36, 0x37, 0x38, 0x39};
    short result = board->readLightSensor();

    if (result != -1) {
      msg[0] = ascii_nums[result / 100];
      msg[1] = ascii_nums[(result / 10) % 10];
      msg[2] = ascii_nums[result % 10];
      msg[3] = '%';
      return true;
    } else {
      strcpy(msg, "light sensor is not calibrated");
      return false;
    }
  };

  void light_sensor_state_transition() {
    board->light_sensor_state_transition();
  };
};
