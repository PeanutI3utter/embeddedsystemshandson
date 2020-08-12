#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define STM32F030xC
#include "stm32f0xx.h"

enum { on, off };

typedef struct led {
  uint32_t on;
  uint32_t off;
} LED;

const LED led1 = {GPIO_BSRR_BS_1, GPIO_BSRR_BR_1};
const LED led2 = {GPIO_BSRR_BS_0, GPIO_BSRR_BR_0};

typedef struct status {
  unsigned char current_LED;
  unsigned char on_off;
  unsigned short toggle_rate;
} Status;

LED leds[2];
Status status;

int main() {
  leds[0] = led1;
  leds[1] = led2;
  status.current_LED = 0;
  status.toggle_rate = 50000;
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;   // AHB peripheral clock enable for GPIO B
  GPIOB->MODER |= GPIO_MODER_MODER1_0; // General purpose output mode for Pin 1
  GPIOB->MODER |= GPIO_MODER_MODER0_0; // General purpose output mode for Pin 1
  GPIOB->MODER &=
      ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER11 | GPIO_MODER_MODER12 |
        GPIO_MODER_MODER13);          // Input mode for PB10, PB11, PB12, PB13
  GPIOB->OTYPER &= ~GPIO_OTYPER_OT_1; // Output type push pull
  GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR1; // low speed for led gpio
  GPIOB->PUPDR &=
      ~(GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR10 | GPIO_PUPDR_PUPDR11 |
        GPIO_PUPDR_PUPDR12 | GPIO_PUPDR_PUPDR13); // no pull up or pull down
  GPIOB->PUPDR |= GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR11_0 |
                  GPIO_PUPDR_PUPDR12_0 | GPIO_PUPDR_PUPDR13_0;
  GPIOB->BRR = GPIO_BRR_BR_1; // reset LED

  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // APB peripheral clock enable for TIM3

  TIM3->SMCR &= ~TIM_SMCR_SMS; // Disable Slave mode that the counter is clocked
                               // by the internal clock
  TIM3->CR1 &= ~TIM_CR1_DIR;   // set upcounting mode
  TIM3->PSC = (unsigned short)80; // set prescaler to 80 so that counter
                                  // frequency is lowered to 100kHz
  TIM3->ARR = status.toggle_rate; // 50000 clocks are necessairy to create an
                                  // overflow every 0.5s
  TIM3->DIER |= TIM_DIER_UIE;

  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PB |
                       SYSCFG_EXTICR3_EXTI11_PB; // map port b to exti 10 & 11
  SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PB |
                       SYSCFG_EXTICR4_EXTI13_PB; // map port b to exti 12 & 13
  EXTI->IMR |= EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR12 |
               EXTI_IMR_MR13; // mask EXTI lines that generate an interrupt
  EXTI->RTSR |= EXTI_FTSR_TR10 | EXTI_FTSR_TR11 | EXTI_FTSR_TR12 |
                EXTI_FTSR_TR13; // triggered on rising edge

  __asm volatile("cpsie i"); // enable IRQ interrupts
  NVIC->ISER[0] =
      (1 << EXTI4_15_IRQn); // enable TIM3 and EXTI4_15 IRQ Handlers 00010080
  NVIC->ISER[0] = (1 << TIM3_IRQn);
  NVIC->IP[1] |= 0x40000000U; // Set Priority of EXTI4_15_IRQ to 64
  NVIC->IP[4] &= ~(0xE0U);    // Set Priority of TIM3 to 0

  TIM3->CR1 |= TIM_CR1_CEN; // enable timer

  while (1)
    ;
}


/*
  Interrupt Handler of TIM3 interrupts
*/
void TIM3_IRQHandler() {
  GPIOB->BSRR = (status.on_off == on ? leds[status.current_LED].off
                                     : leds[status.current_LED].on);  // toggles the LED
  status.on_off = status.on_off == on ? off : on;                     // toggle on/off state
  TIM3->SR &= ~TIM_SR_UIF;                                            // reset the interrupt flag
}

/*
Interrupt Handler for External Input Interrupts(GPIOs in this case)
*/
void EXTI4_15_IRQHandler() {
  uint32_t PR = EXTI->PR;                                 // read set interrupts
  EXTI->PR = 0xFFFFFFFF;                                  // reset interrupt flags
  NVIC->ICPR[0] = (1U << 7);                              // reset EXTI pending flag in NVIC

  int highestPending = -1;
  while (PR) {                                            // get highest interrupt
    highestPending++;
    PR = PR >> 1;
  }

  if (highestPending == 11 || highestPending == 13) {     // (UP/DONW) toggle between LEDs
    GPIOB->BSRR = leds[status.current_LED].off;
    status.current_LED = (status.current_LED + 1) & 1U;
  } else if (highestPending == 10) {                      // (LEFT) decrease toggle rate
    if (status.toggle_rate > 5000) {
      status.toggle_rate -= 5000;
      TIM3->ARR = status.toggle_rate;
    }
  } else if (highestPending == 12) {                      // (RIGHT) increase toggle rate
    if (status.toggle_rate < 60000) {
      status.toggle_rate += 5000;
      TIM3->ARR = status.toggle_rate;
    }
  }
}

