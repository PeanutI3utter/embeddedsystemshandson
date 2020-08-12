/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#include <ch.hpp>
#include <cstdint>
#include <hal.h>

using namespace chibios_rt;

static uint8_t buffer;
static volatile bool recdone = false;
static volatile bool senddone = false;

/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void txend1(UARTDriver *uartp) { (void)uartp; }

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp) {
  (void)uartp;
  senddone = true; // if send is done set send semophore to valid
}

/*
 * This callback is invoked when a character is received but the application
 * was not ready to receive it, the character is passed as parameter.
 */
static void rxchar(UARTDriver *uartp, uint16_t c) {
  (void)uartp;
  (void)c;
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp) {
  (void)uartp;
  recdone = true; // if receipt is done set receipt semophore to valid
}

static void rxerr(UARTDriver *uartp, uartflags_t e) {
  (void)uartp;
  (void)e;
}

static void timeout(UARTDriver *uartp) { (void)uartp; }

static UARTConfig uart_cfg = {
    txend1, // callback when end of transmission buffer has been reached
    txend2, // callback when transmission has ended physically
    rxend,  // callback when end of receive buffer has been reached
    rxchar, // callback when character received in idle state
    rxerr,  // callback when error while receiving
    timeout,
    0,      // timeout
    921600, // Bit rate
    0,      // contents of CR1 register
    0,      // contents of CR2 register
    0       // contents of CR3 register
};

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  System::init();

  uartStart(
      &UARTD5,
      &uart_cfg); // start UART 5 driver with configuration given by uart_cfg

  palSetPadMode(GPIOB, GPIOB_LED1_GREEN,
                PAL_MODE_OUTPUT_PUSHPULL); // set GPIO mode for LED 1
  palSetPadMode(GPIOB, GPIOB_LED0_RED,
                PAL_MODE_OUTPUT_PUSHPULL); // set GPIO mode for LED 0

  palSetPad(GPIOB, GPIOB_LED1_GREEN); // green for ready to receive/receiving
  palClearPad(GPIOB, GPIOB_LED0_RED); // red for busy/sending

  while (true) {
    uartStartReceive(&UARTD5, 1, &buffer); // Start receiving 1 byte
    while (!recdone)
      ; // as long as receive not done halt here
    recdone = false;
    uartStartSend(&UARTD5, 1, &buffer); // send received buffer back
    while (!senddone)
      ; // wait until send is done
    senddone = false;
  }

  return 0;
}
