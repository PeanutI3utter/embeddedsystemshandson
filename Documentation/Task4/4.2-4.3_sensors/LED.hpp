#ifndef LED_H

#define LED_H

#include <ch.h>
#include <hal.h>

class LED {
public:
  LED(stm32_gpio_t *_port, unsigned int _pin);
  void off();
  void on();
  void toggle();

private:
  stm32_gpio_t *port;
  unsigned int pin;
};

#endif