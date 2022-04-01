/*
 * can.h
 *
 *  Created on: 21 Sep 2021
 *      Author: Michal Makowka
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_

#include <stdio.h>
#include <string.h>
#include "stm32f303x8.h"


#define STANDARD_FORMAT  0
#define EXTENDED_FORMAT  1

#define DATA_FRAME       0
#define REMOTE_FRAME     1

#define CAN_ID_STD		 0
#define CAN_ID_EXT		 0
#define CAN_RTR_DATA	 0
#define CAN_RTR_REMOTE	 0


typedef struct {
	uint id;			// Node identifier
	char data[8];		// Data field
	uint len;			// Length of data in bytes
	uint format;		// Standard (0), Extended ID (1)
	uint type;			// Data frame (0), Remote frame (1)
} CAN_MESSAGE;


CAN_MESSAGE can_rx_message;



void CanInit(void);
void Can_Tx_Msg(CAN_MESSAGE * msg);
void Can_Rx_Msg(CAN_MESSAGE * msg);
void Can_Set_Filter(uint id, char format);

#endif /* INC_CAN_H_ */
