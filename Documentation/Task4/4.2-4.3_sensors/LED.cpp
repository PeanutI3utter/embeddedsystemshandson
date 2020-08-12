#include "LED.hpp"

LED::LED(stm32_gpio_t *_port, unsigned int _pin) {
  port = _port;
  pin = _pin;
}

void LED::on() { palSetPad(port, pin); }

void LED::off() { palClearPad(port, pin); }

void LED::toggle() { palTogglePad(port, pin); }