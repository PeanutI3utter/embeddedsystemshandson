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

#define DIMMINGTASK // if subtask is about dimming LED

#include <ch.hpp>
#include <hal.h>
#include <math.h> 

using namespace chibios_rt;


static const uint16_t PWM_F = 10000; // PWM frequency
static const uint16_t ARR = 100;     // Auto Reload Register value

#ifdef DIMMINGTASK 
static uint16_t dc = 0;                                   // current duty cycle

static const float VDD = 3.3f;                            
static const float V_HIGH = 2.5f;                         // Voltage of high brightness
static const float V_LOW = 0.5f;                          // Voltage of low brightness


static const uint16_t DIMM_F = 1;                         // dimming frequency
static const uint16_t OUT_F = PWM_F / ARR;                // output frequency
static const uint16_t TICKS_PER_PERIOD = OUT_F / DIMM_F;  // ticks needed by output signal to reach one period
static uint16_t tick = 0;

#else

static uint16_t duty_cycle[] = {1515, 5000, 10000};       // 0.5 V, 1.65 V, 3.3 V
static uint8_t selection = 0;
#endif

static PWMDriver *pwmdriver = &PWMD3;

static void pwmpcb(PWMDriver *pwmp) {

  (void)pwmp;
}

static void pwmc1cb(PWMDriver *pwmp) {

  (void)pwmp;
#ifdef DIMMINGTASK
  dc = (uint16_t)((V_LOW  + (V_HIGH - V_LOW) * (1.0f - abs(1.0f - 2.0f * (float)tick / TICKS_PER_PERIOD))) * 10000 / VDD); // triangle function
  tick = tick + 1 > TICKS_PER_PERIOD ? 0 : tick + 1;                                                                       // update tick
#endif

}


static uint32_t pwmChan = 3;                // TIM3 channel 4

static PWMConfig pwmcfg = {
  PWM_F,                                    // PWM frequency setting 
  ARR,                                      
  pwmpcb,
  {
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb}        // options for channel 4 (green LED)
  },
  0,
  0
};


#ifndef DIMMINGTASK
static void joystickUp(void *args)
{

  selection = selection + 1 <= 2 ? selection + 1 : 2;
  
}

static void joystickDown(void * args)
{
  selection = selection - 1 >= 0 ? selection - 1 : 0;
}
#endif
/*
 * Application entry point.
 */
int main(void) {
  halInit();
  System::init();

#ifndef DIMMINGTASK
  // define callbacks on GPIO interrupts
  palEnablePadEvent(GPIOB, 11, PAL_EVENT_MODE_RISING_EDGE);
  palSetPadCallback(GPIOB, 11, joystickUp, NULL);

  palEnablePadEvent(GPIOB, 13, PAL_EVENT_MODE_RISING_EDGE);
  palSetPadCallback(GPIOB, 13, joystickDown, NULL);
#endif

  pwmStart(pwmdriver, &pwmcfg);
  pwmEnablePeriodicNotification(pwmdriver);


#ifdef DIMMINGTASK
  pwmEnableChannel(pwmdriver, pwmChan, PWM_PERCENTAGE_TO_WIDTH(pwmdriver, dc));                      // set PWM to default duty cylcle
#else
  pwmEnableChannel(pwmdriver, pwmChan, PWM_PERCENTAGE_TO_WIDTH(pwmdriver, duty_cycle[selection]));
#endif

  pwmEnableChannelNotification(pwmdriver, pwmChan);
  
  palSetPadMode(GPIOB, 1, PAL_MODE_ALTERNATE(1));                                                    // set GPIOB1 to AF2 which is connected to TIM3 output

  while (true) {
    #ifdef DIMMINGTASK                                                                               // update PWM duty cycke
    pwmEnableChannel(pwmdriver, pwmChan, PWM_PERCENTAGE_TO_WIDTH(pwmdriver, dc));
    #else
    pwmEnableChannel(pwmdriver, pwmChan, PWM_PERCENTAGE_TO_WIDTH(pwmdriver, duty_cycle[selection]));
    #endif
    chThdSleepMilliseconds(50);
  }
  return 0;
}
