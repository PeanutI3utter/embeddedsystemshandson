#define STM32F030xC

#include "stm32f0xx.h"

int main(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // AHB peripheral clock enable for GPIO B

  GPIOB->MODER |= GPIO_MODER_MODER1_0; //
  GPIOB->OTYPER &= ~GPIO_OTYPER_OT_1;
  GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1;
  GPIOB->PUPDR |= GPIO_PUPDR_PUPDR1;
  GPIOB->BRR = GPIO_BRR_BR_1; // reset LED

  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // APB peripheral clock enable for TIM3

  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // APB peripheral clock enable for TIM3
  TIM3->SMCR &= ~TIM_SMCR_SMS; // Disable Slave mode that the counter is clocked
                               // by the internal clock
  TIM3->CR1 &= ~TIM_CR1_DIR;   // set upcounting mode
  TIM3->PSC = (unsigned short)80; // set prescaler to 480 so that counter
                                  // frequency is lowered to 100kHz
  TIM3->ARR =
      50000; // 50000 clocks are necessairy to create an overflow every 0.5s
  TIM3->CR1 |= TIM_CR1_CEN; // enable timer
  int i = 0;
  int onoff = 0;
  while (1) {
    if (TIM3->SR & TIM_SR_UIF) {
      TIM3->SR &= ~TIM_SR_UIF; // set update interrupt flag to 0
      // toggle led
      if (++onoff % 2 == 1) {
        GPIOB->BSRR = GPIO_BSRR_BS_1;
      } else {
        GPIOB->BRR |= GPIO_BRR_BR_1;
      }
    }
  }

  return 0;
}
