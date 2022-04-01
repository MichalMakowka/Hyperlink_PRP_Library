/*
 * config.c
 *
 *  Created on: Jan 31, 2022
 *      Author: Michal Makowka
 */
#include "config.h"


void SystemCFG (void) {

	/*
	 * Timer Config Formula
	 * (PSC+1)*(ARR+1) = TIMclk/Updatefrequency
	 * =64 000 000 / 10 000Hz desired
	 * PSC = 639 | ARR = 9
	 */

	// System CLK 36MHz
	RCC->CFGR |= RCC_CFGR_PLLMUL9;
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	RCC->CR |= RCC_CR_PLLON;
	FLASH->ACR |= FLASH_ACR_LATENCY_1;
	while (!(RCC->CR & RCC_CR_PLLRDY));
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);


/*	// Soft-PWM Timer and interrupt Config (LIM CHANNEL A)
	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
	TIM15->CCMR1 &= ~(TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC1S_1);	// CC1 channel configured as output
	TIM15->CCMR1 |= TIM_CCMR1_OC1M_0;
	TIM15->CCMR1 &= ~(TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_3);	// Set channel 1 to active level on match.
	TIM15->DIER |= TIM_DIER_CC1IE;	// Capture/compare 1 interrupt EN
	TIM1->EGR = TIM_EGR_UG;
	TIM15->PSC = 9;
	TIM15->ARR = 639;
	TIM15->CCER |= TIM_CCER_CC1E;	// Channel EN

	NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);


	TIM15->CCR1 =200;
	cnt = 0;

	TIM15->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;	// TIM start and buffering EN
*/
	// Hardware PWM Timer and interrupt Config (LIM CHANNEL B)




	// RCC clock EN configuration
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;	// GPIO Clk Enable
	RCC->APB1ENR |= RCC_APB1ENR_CANEN;												// CAN Clk Enable

	// Status LEDs GPIO config
	GPIOC->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0;	// GPIO Output mode

	// LIM1 Pinout
	// Pins as output
	GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER15_0;
	GPIOA->MODER &= ~(GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1 | GPIO_MODER_MODER2_1 | GPIO_MODER_MODER15_1);
	GPIOB->MODER |= GPIO_MODER_MODER3_0 | GPIO_MODER_MODER10_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER13_0;
	GPIOB->MODER &= ~(GPIO_MODER_MODER3_1 | GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1 | GPIO_MODER_MODER13_1);
	// Fault Pin
	GPIOB->MODER &= ~(GPIO_MODER_MODER12_0 | GPIO_MODER_MODER12_1);		// Input
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR12_0;	// Pull-Up

	// LIM2 Pinout
	// Pins as output
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0;
	GPIOC->MODER &= ~(GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1 | GPIO_MODER_MODER8_1);
	GPIOA->MODER |= GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0;
	GPIOA->MODER &= ~(GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);
	// Fault Pin
	GPIOB->MODER &= ~(GPIO_MODER_MODER14_0 | GPIO_MODER_MODER14_1);		// Input
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR14_0;	// Pull-Up

	// CAN GPIO Pins
	// CAN bus GPIO config
	GPIOB->MODER |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1;	// GPIO PB8 and PB9 Alternative Mode (CANbx)
	GPIOB->AFR[1] |= (0b1001 << 0) | (0b1001 << 4);	// PB8 and PB9 AF set to CAN_Tx and CAN_Rx

	SysTick_Config(3600000000/100000);
	// Reset the SysTick counter value.
	SysTick->VAL = 0UL;
	// Set SysTick source and IRQ.
	SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk);
}

void delay_ms(int ms) {
	// Enable the SysTick timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	// Wait for a specified number of milliseconds
	delay = 0;
	while (delay < ms);

	// Disable the SysTick timer
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/*
 __attribute__((interrupt)) void TIM1_BRK_TIM15_IRQHandler(void){
	if(TIM15->SR & TIM_SR_CC1IF) {
		TIM15->SR &= ~TIM_SR_CC1IF;
		// Interrupt body code here...
	}
}

 */


__attribute__((interrupt)) void SysTick_Handler(void){
	delay++;
}
