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
#include "utils.hpp"

// Board Header Files
#include "KPS3227SP1C.hpp"
#include "LSM303D.hpp"
#include "LSM6DS33.hpp"
#include "SensorProcessor.hpp"

enum REQUEST_CODE { ACCELEROMETER = 1, MAGNETOMETER = 2, LIGHT_SENSOR = 3 };

// CHANGE ACCORDING TO YOUR BOARD VERSION! 3 for version 3 (LSM303D), 4 for
// version 4 (LSM6DS33)
#define BOARD_VERSION 3

// Board initialization
SensorProcessor *processor;

// Namespace
using namespace chibios_rt;

// UART semaphore config
volatile bool receive_semaphore = false;
volatile bool send_semaphore = false;

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
  send_semaphore = true; // if send is done set send semophore to valid
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
  receive_semaphore = true;
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
    0,     // timeout
    115200, // Bit rate
    0,     // contents of CR1 register
    0,     // contents of CR2 register
    0      // contents of CR3 register
};

static const I2CConfig i2ccfg = {
    STM32_TIMINGR_PRESC(5) | STM32_TIMINGR_SCLH(3) | STM32_TIMINGR_SCLL(9) |
        STM32_TIMINGR_SCLDEL(3) | STM32_TIMINGR_SDADEL(3),
    0, 0};

static void adcerrorcallback(ADCDriver *adcp, adcerror_t err) {
  (void)adcp;
  (void)err;
}

static void adccallback(ADCDriver *adcp) { (void)adcp; }

static const ADCConversionGroup adcgrpcfg1 = {
    TRUE,
    1,
    adccallback,
    adcerrorcallback,
    ADC_CFGR1_CONT | ADC_CFGR1_RES_12BIT, /* CFGR1 */
    ADC_TR(0, 0),                         /* TR */
    ADC_SMPR_SMP_1P5,                     /* SMPR */
    ADC_CHSELR_CHSEL6                     /* CHSELR */
};

void onButtonClick(void *arg) {
  (void)arg;
  processor->light_sensor_state_transition();
}

/*
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific , buffer);
  recdone = false;initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  System::init();

  // Start UART communication
  uartStart(&UARTD5, &uart_cfg);

  // Start i2c communication
  i2cStart(&I2CD1, &i2ccfg);

  adcStart(&ADCD1, nullptr);

  // Board initialization
  LED greenLED(GPIOB, GPIOB_LED1_GREEN);
  LED redLED(GPIOB, GPIOB_LED0_RED);
  KPS3227SP1C kps3227sp1c(&greenLED, &redLED, &ADCD1);
  kps3227sp1c.sensorADC = &adcgrpcfg1;

#if BOARD_VERSION == 3
  LSM303D board(&kps3227sp1c);
#else
  LSM6DS33 board(&kps3227sp1c);
#endif

  SensorProcessor p1 = SensorProcessor(&board);
  processor = &p1;

  // set callback on button push
  palEnablePadEvent(GPIOB, 2, PAL_EVENT_MODE_FALLING_EDGE);
  palSetPadCallback(GPIOB, 2, onButtonClick, NULL);

  // Handle commands
  while (true) {
    REQUEST_CODE command_buffer;

    uartStartReceive(&UARTD5, 1, &command_buffer);
    while (!receive_semaphore)
      ;
    receive_semaphore = false;

    int msg_length = 41;
    char msg[msg_length];
    switch (command_buffer) {
    case ACCELEROMETER:
      processor->readAccelerometer(msg);
      break;
    case MAGNETOMETER:
      processor->readMagnetometer(msg);
      break;
    case LIGHT_SENSOR:
      if (processor->readLightSensor(msg)) {
        msg_length = 5;
        break;
      }
      msg_length = 31;
      break;
    default:
      strcpy(msg, "error, wrong command!");
      msg_length = 21;
    }

    // Replace String terminator by new line
    int nullByteStopIndex = msg_length - 1;
    for (int i = 0; i < msg_length; i++) {
      if (msg[i] == '\0') {
        nullByteStopIndex = i;
        break;
      }
    }

    msg[nullByteStopIndex] = '\n';
    uartStartSend(&UARTD5, nullByteStopIndex + 1, msg);
    while (!send_semaphore)
      ;
    send_semaphore = false;
  }

  return 0;
}
