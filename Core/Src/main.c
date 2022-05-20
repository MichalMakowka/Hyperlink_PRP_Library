/*
 * main.c
 *
 *	Hyperlink Propulsion System Library
 *
 *  Created on: 31 Jan 2022
 *  Author: Michal Makowka
 *
 *  v1.0
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


void VA_H (void) {B_H1_OFF; B_L2_ON;}
void VA_L (void) {B_H1_ON; B_L2_OFF;}
void VB_H (void) {B_H3_OFF; B_L4_ON;}
void VB_L (void) {B_H3_ON; B_L4_OFF;}
void VC_H (void) {B_H5_OFF; B_L6_ON;}
void VC_L (void) {B_H5_ON; B_L6_OFF;}

void PhaseExec(uint8_t val);

void Sine_xTOx(void(*A)(void), void(*B)(void), uint8_t T_100, uint8_t T_93_3, uint8_t T_75, uint8_t T_50);

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


	//char speedStr[2];
	while (1) {
/*
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
*/
		// Testing value
		PhaseExec(20);
	}
}


void PhaseExec(uint8_t val) {	// Default val = 20
	uint8_t i;

		// *** 3-Phase H-Bridge Driving Circuit Sequence with SoftPWM *** //
		// S6 - 101 TO S1 - 001
		// V_c constant HI AND V_b constant LOW
		VC_H();
		VB_L();
	for (i=0; i<val; i++) {
		// V_a HI -> to LOW
		VA_L();
		delay_ms(i);
		VA_H();
		delay_ms(val-i);
	}
//		Sine_xTOx(VA_H, VA_L, 10, 9, 7, 5);

		// S1 - 001 TO S2 - 011
		// V_a constant LOW AND V_c constant HI
		VA_L();
		VC_H();
	for (i=0; i<val; i++) {
		// V_b LOW -> to HI
		VB_H();
		delay_ms(i);
		VB_L();
		delay_ms(val-i);
	}
//		Sine_xTOx(VB_L, VB_H, 10, 9, 7, 5);

		// S2 - 011 TO S3 - 010
		// V_a constant LOW AND V_b constant HI
		VA_L();
		VB_H();
	for (i=0; i<val; i++) {
		// V_c HI -> to LOW
		VC_L();
		delay_ms(i);
		VC_H();
		delay_ms(val-i);
	}
//		Sine_xTOx(VC_H, VC_L, 10, 9, 7, 5);

		// S3 - 010 TO S4 - 110
		// V_b constant HI AND V_c constant LOW
		VB_H();
		VC_L();
	for (i=0; i<val; i++) {
		// V_a LOW -> to HI
		VA_H();
		delay_ms(i);
		VA_L();
		delay_ms(val-i);
	}
//		Sine_xTOx(VA_L, VA_H, 10, 9, 7, 5);
		// S4 - 110 TO S5 - 100
		// V_a constant HI AND V_c constant LOW
		VA_H();
		VC_L();
	for (i=0; i<val; i++) {
		// V_b HI -> to LOW
		VB_L();
		delay_ms(i);
		VB_H();
		delay_ms(val-i);
	}
//		Sine_xTOx(VB_H, VB_L, 10, 9, 7, 5);

		// S5 - 100 TO S6 - 101
		// V_a constant HI AND V_b constant LOW
		VA_H();
		VB_L();
	for (i=0; i<val; i++) {
		// V_c LOW -> to H
		VC_H();
		delay_ms(i);
		VC_L();
		delay_ms(val-i);
	}
//		Sine_xTOx(VC_L, VC_H, 10, 9, 7, 5);

}

/*
FIRST VERSION
// H-Bridge Driving Circuit Sequence (1-Phase)
		B_H3_ON;
		B_L2_ON;
	for (i=1; i<val; i++) {
		B_H1_OFF;
		B_L4_OFF;
		delay_ms(moddelay);
		B_H1_ON;
		B_L4_ON;
		delay_ms(val-moddelay);
	}
	for (i=val; i>0; i--) {
		B_H1_OFF;
		B_L4_OFF;
		delay_ms(moddelay);
		B_H1_ON;
		B_L4_ON;
		delay_ms(val-moddelay);
	}
		B_H1_ON;
		B_L4_ON;
	for (i=1; i<val; i++) {
		B_H3_OFF;
		B_L2_OFF;
		delay_ms(moddelay);
		B_H3_ON;
		B_L2_ON;
		delay_ms(val-moddelay);
		}
	for (i=val; i>0; i--) {
		B_H3_OFF;
		B_L2_OFF;
		delay_ms(moddelay);
		B_H3_ON;
		B_L2_ON;
		delay_ms(val-moddelay);
	}
}

 */


//// H-Bridge Driving Circuit Sequence (1-Phase)
// SECOND VERSION
//	for (i=1; i<val; i++) {
//		// V_b H transitiones to L
//		VB_L;
//		delay_ms(moddelay);
//		VB_H;
//		delay_ms(val-moddelay);
//	}
//		// V_b on GND
//		VB_L;
//
//
//	for (i=1; i<val; i++) {
//		// V_a L transitiones to H
//		VA_H;
//		delay_ms(moddelay);
//		VA_L;
//		delay_ms(val-moddelay);
//		}
//		// V_a on VCC
//		VA_H;
//
///*	for (i=1; i<val; i++) {
//		// V_b L transitiones to H
//		VB_H;
//		delay_ms(moddelay);
//		VB_L;
//		delay_ms(val-moddelay);
//	}
//		// V_b on VCC
//		VB_H;
//*/
//	for (i=1; i<val; i++) {
//		// V_a H transitiones to L
//		VA_L;
//		delay_ms(moddelay);
//		VA_H;
//		delay_ms(val-moddelay);
//		}
//		// V_a on GND
//		VA_L;
//


void Sine_xTOx(void(*A)(void), void(*B)(void), uint8_t T_100, uint8_t T_93_3, uint8_t T_75, uint8_t T_50) {
	// Active A 100%
	(*A)();
	delay_ms(T_100);

	// Active A 93.3%
	(*A)();
	delay_ms(T_93_3);
	(*B)();
	delay_ms(T_100-T_93_3);

	// Active A 75%
	(*A)();
	delay_ms(T_75);
	(*B)();
	delay_ms(T_100-T_75);

	// Active A 50%
	(*A)();
	delay_ms(T_50);
	(*B)();
	delay_ms(T_50);

	// Active A 25%
	(*A)();
	delay_ms(T_100 - T_75);
	(*B)();
	delay_ms(T_75);

	// Active A 6.6%
	(*A)();
	delay_ms(T_100 - T_93_3);
	(*B)();
	delay_ms(T_93_3);

	// Active A 0%
	(*B)();
	delay_ms(T_100);
}
