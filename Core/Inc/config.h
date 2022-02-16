/*
 * config.h
 *
 *  Created on: Jan 31, 2022
 *      Author: Michal Makowka
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include <stdio.h>
#include <string.h>
#include "stm32f303x8.h"

#define A_H1_OFF	GPIOA->ODR &= ~GPIO_ODR_0
#define A_H1_ON	GPIOA->ODR |=  GPIO_ODR_0
#define A_H3_OFF	GPIOA->ODR &= ~GPIO_ODR_1
#define A_H3_ON	GPIOA->ODR |=  GPIO_ODR_1
#define A_H5_OFF	GPIOA->ODR &= ~GPIO_ODR_2
#define A_H5_ON	GPIOA->ODR |=  GPIO_ODR_2
#define A_L2_OFF	GPIOA->ODR &= ~GPIO_ODR_15
#define A_L2_ON	GPIOA->ODR |=  GPIO_ODR_15
#define A_L4_OFF	GPIOB->ODR &= ~GPIO_ODR_3
#define A_L4_ON	GPIOB->ODR |=  GPIO_ODR_3
#define A_L6_OFF	GPIOB->ODR &= ~GPIO_ODR_10
#define A_L6_ON	GPIOB->ODR |=  GPIO_ODR_10

#define B_H1_OFF	GPIOC->ODR &= ~GPIO_ODR_6
#define B_H1_ON	GPIOC->ODR |=  GPIO_ODR_6
#define B_H3_OFF	GPIOC->ODR &= ~GPIO_ODR_7
#define B_H3_ON	GPIOC->ODR |=  GPIO_ODR_7
#define B_H5_OFF	GPIOC->ODR &= ~GPIO_ODR_8
#define B_H5_ON	GPIOC->ODR |=  GPIO_ODR_8
#define B_L2_OFF	GPIOA->ODR &= ~GPIO_ODR_8
#define B_L2_ON	GPIOA->ODR |=  GPIO_ODR_8
#define B_L4_OFF	GPIOA->ODR &= ~GPIO_ODR_9
#define B_L4_ON	GPIOA->ODR |=  GPIO_ODR_9
#define B_L6_OFF	GPIOA->ODR &= ~GPIO_ODR_10
#define B_L6_ON	GPIOA->ODR |=  GPIO_ODR_10



volatile int delay;
void delay_ms(int ms);

volatile uint8_t cnt;




void SystemCFG (void);


#endif /* INC_CONFIG_H_ */
