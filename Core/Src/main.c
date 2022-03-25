/*
 * main.c
 *
 *	Hyperlink Propulsion System Library
 *
 *  Created on: 31 Jan 2022
 *  Author: Michal Makowka
 *
 *  v1.1
 *
 *  (C) All rights reserved.
 *
 *  https://mmttechnologies.com/
 */
#include <stdio.h>
#include <string.h>
#include "stm32f303x8.h"
#include "config.h"


int main(void) {




	SystemCFG();
	GPIOC->ODR |= GPIO_ODR_0 | GPIO_ODR_1 | GPIO_ODR_2 | GPIO_ODR_3;	// Clear LEDs
	// Clear


	GPIOB->ODR |= GPIO_ODR_11;		// STDrive Enable
	GPIOB->ODR |= GPIO_ODR_13;		// STDrive Enable

	B_H1_OFF;
	B_L2_OFF;
	B_H3_OFF;
	B_L4_OFF;
	B_H5_OFF;
	B_L6_OFF;

	uint8_t i;

	while (1) {

	// H-Bridge Driving Circuit Sequence (1-Phase)
		B_H3_ON;
		B_L2_ON;
	for (i=1; i<20; i++) {
		B_H1_OFF;
		B_L4_OFF;
		delay_ms(i);
		B_H1_ON;
		B_L4_ON;
		delay_ms(20-i);
	}
	for (i=20; i>0; i--) {
		B_H1_OFF;
		B_L4_OFF;
		delay_ms(i);
		B_H1_ON;
		B_L4_ON;
		delay_ms(20-i);
	}
		B_H1_ON;
		B_L4_ON;
	for (i=1; i<20; i++) {
		B_H3_OFF;
		B_L2_OFF;
		delay_ms(i);
		B_H3_ON;
		B_L2_ON;
		delay_ms(20-i);
		}
	for (i=20; i>0; i--) {
		B_H3_OFF;
		B_L2_OFF;
		delay_ms(i);
		B_H3_ON;
		B_L2_ON;
		delay_ms(20-i);
	}



/*
	A_H5_OFF;
	A_H1_ON;

	A_L4_OFF;
	A_L6_ON;
	delay_ms(200);
	A_H1_OFF;
	A_H3_ON;

	A_L6_OFF;
	A_L2_ON;
	delay_ms(200);
	A_H3_OFF;
	A_H1_ON;

	A_L2_OFF;
	A_L6_ON;
	delay_ms(200);
	A_H1_OFF;
	A_H5_ON;

	A_L6_OFF;
	A_L4_ON;
	delay_ms(200);
*/

	}
}

