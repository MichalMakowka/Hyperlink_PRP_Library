/*
 * main.c
 *
 *	Hyperlink Propulsion System Library
 *
 *  Created on: 31 Jan 2022
 *  Author: Michal Makowka
 *
 *  v1.3
 *
 *  (C) All rights reserved.
 *
 *  https://mmttechnologies.com/
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f303x8.h"
#include "config.h"
#include "can.h"

void PhaseExec(uint8_t val);


int main(void) {

		CAN_MESSAGE can_status_ok;
			strcpy(can_status_ok.data, "pr_0000");
			can_status_ok.format = STANDARD_FORMAT;
			can_status_ok.id = 0x01;
			can_status_ok.len = sizeof(can_status_ok.data);
			can_status_ok.type = DATA_FRAME;

		CAN_MESSAGE can_status_fault;
			strcpy(can_status_fault.data, "pr_1111");
			can_status_fault.format = STANDARD_FORMAT;
			can_status_fault.id = 0x01;
			can_status_fault.len = sizeof(can_status_fault.data);
			can_status_fault.type = DATA_FRAME;

		CAN_MESSAGE can_ack;
			strcpy(can_ack.data, "pr_p_xx");
			can_ack.format = STANDARD_FORMAT;
			can_ack.id = 0x01;
			can_ack.len = sizeof(can_ack.data);
			can_ack.type = DATA_FRAME;



	SystemCFG();

	GPIOC->ODR |= GPIO_ODR_0 | GPIO_ODR_1 | GPIO_ODR_2 | GPIO_ODR_3;	// Clear LEDs
	// Clear

	CanInit();

	Can_Set_Filter(0x00, STANDARD_FORMAT);
	Can_Set_Filter(0x03, STANDARD_FORMAT);


	GPIOB->ODR |= GPIO_ODR_11;		// STDrive Enable
	GPIOB->ODR |= GPIO_ODR_13;		// STDrive Enable

	B_H1_OFF;
	B_L2_OFF;
	B_H3_OFF;
	B_L4_OFF;
	B_H5_OFF;
	B_L6_OFF;


	char speedStr[2];
	while (1) {

		if (!strcmp(can_rx_message.data, "mb_1010")) {
			memset(can_rx_message.data, 0, sizeof(can_rx_message.data));
			delay_ms(150);
			// Perform status check here
			Can_Tx_Msg(&can_status_ok);
		}
		else if (!strncmp(can_rx_message.data, "mb_p_xx", 5)) {
			speedStr[0] = can_rx_message.data[5];
			speedStr[1] = can_rx_message.data[6];
			PhaseExec(atoi(speedStr));
		}
		else {

		}

	}
}


void PhaseExec(uint8_t val) {	// Default val = 20
	uint8_t i;
	// H-Bridge Driving Circuit Sequence (1-Phase)
		B_H3_ON;
		B_L2_ON;
	for (i=1; i<val; i++) {
		B_H1_OFF;
		B_L4_OFF;
		delay_ms(i);
		B_H1_ON;
		B_L4_ON;
		delay_ms(val-i);
	}
	for (i=val; i>0; i--) {
		B_H1_OFF;
		B_L4_OFF;
		delay_ms(i);
		B_H1_ON;
		B_L4_ON;
		delay_ms(val-i);
	}
		B_H1_ON;
		B_L4_ON;
	for (i=1; i<val; i++) {
		B_H3_OFF;
		B_L2_OFF;
		delay_ms(i);
		B_H3_ON;
		B_L2_ON;
		delay_ms(val-i);
		}
	for (i=val; i>0; i--) {
		B_H3_OFF;
		B_L2_OFF;
		delay_ms(i);
		B_H3_ON;
		B_L2_ON;
		delay_ms(val-i);
	}
}


