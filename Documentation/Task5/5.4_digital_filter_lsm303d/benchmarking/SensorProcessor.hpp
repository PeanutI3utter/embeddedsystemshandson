#include "Board.hpp"
#include <cstring>

typedef struct {
  float x;
  float y;
  float z;
} axis;

// Configure places after point in values
const int PLACES = 8;

class SensorProcessor {
private:
  Board *board;

  // Divide read bits and calculate conversion factor
  const int intervall_size = 32768;
  // +-2 g: ~0.061 mg/LSB
  const double conversion_factor_acc = 2.0 / intervall_size;

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
   * Generate string from float with PLACES decimal places after comma
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

    for (int i = 0; i < PLACES; i++) {
      value = (value - (int)value) * 10;
      str[start++] = '0' + (int)value;
    }

    str[start++] = '\0';
  }

  /**
   * Generate human readable String
   */
  void generateResponseString(axis *axis_data, char msg[]) {
    char x[5 + PLACES];
    ftoa(axis_data->x, x);

    char y[5 + PLACES];
    ftoa(axis_data->y, y);

    char z[5 + PLACES];
    ftoa(axis_data->z, z);

    strcpy(msg, "{ \"X\": ");
    strcat(msg, x);
    strcat(msg, ", \"Y\": ");
    strcat(msg, y);
    strcat(msg, ", \"Z\": ");
    strcat(msg, z);
    strcat(msg, " }");
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
};
