/*
 * can.c
 *
 *  Created on: 21 Sep 2021
 *      Author: Michal Makowka
 */

#include "can.h"


void CanInit(void) {


	CAN->MCR |= CAN_MCR_INRQ;			// Initialisation mode on

	// *** CAN Test Mode *** //
//	CAN1->BTR |= CAN_BTR_LBKM;			// Loopback mode enabled (For Debug purposes)
	// ********************* //

	NVIC_EnableIRQ(CAN_TX_IRQn);
	NVIC_EnableIRQ(CAN_RX0_IRQn);

	CAN->IER |= CAN_IER_FMPIE0;			// FIFO message pending interrupt enable

	CAN->BTR = 0x00070004;				// CAN Bitrate: 2010000 (data for the logic analyser debug purposes)

	CAN->MCR &= ~CAN_MCR_INRQ;			// Initialisation mode off
	while (CAN->MSR & CAN_MCR_INRQ);



	CAN->MCR &= ~CAN_MCR_SLEEP;


}


void Can_Tx_Msg(CAN_MESSAGE * msg) {

	CAN->sTxMailBox[0].TIR = 0;		// Empty TIR register

	if(msg->format == STANDARD_FORMAT)	{
		CAN->sTxMailBox[0].TIR |= (u_int) (msg->id << 21) | CAN_ID_STD;
	} else {
		CAN->sTxMailBox[0].TIR |= (u_int) (msg->id << 3) | CAN_ID_EXT;
	}

	if (msg->type == DATA_FRAME)  {                 // DATA FRAME
	    CAN->sTxMailBox[0].TIR |= CAN_RTR_DATA;
	} else {                                        // REMOTE FRAME
	    CAN->sTxMailBox[0].TIR |= CAN_RTR_REMOTE;
	}

	CAN->sTxMailBox[0].TDLR = (((u_int)msg->data[3] << 24) |
	                          ((u_int)msg->data[2] << 16) |
	                          ((u_int)msg->data[1] <<  8) |
	                          ((u_int)msg->data[0]));
	CAN->sTxMailBox[0].TDHR = (((u_int)msg->data[7] << 24) |
	                          ((u_int)msg->data[6] << 16) |
	                          ((u_int)msg->data[5] <<  8) |
	                          ((u_int)msg->data[4]));

	CAN->sTxMailBox[0].TDTR &= ~CAN_TDT1R_DLC;
	CAN->sTxMailBox[0].TDTR |=  (msg->len & CAN_TDT0R_DLC);

	CAN->IER |= CAN_IER_TMEIE;                      // enable  TME interrupt
	CAN->sTxMailBox[0].TIR |=  CAN_TI0R_TXRQ;       // transmit message

}


void Can_Rx_Msg(CAN_MESSAGE * msg) {

	if ((CAN->sFIFOMailBox[0].RIR & CAN_ID_EXT) == 0) { // Standard ID
	    msg->format = STANDARD_FORMAT;
	    msg->id     = (uint32_t)0x000007FF & (CAN->sFIFOMailBox[0].RIR >> 21);
	  }  else  {                                          // Extended ID
	    msg->format = EXTENDED_FORMAT;
	    msg->id     = (uint32_t)0x0003FFFF & (CAN->sFIFOMailBox[0].RIR >> 3);
	  }
	                                                  // Read type information
	  if ((CAN->sFIFOMailBox[0].RIR & CAN_RTR_REMOTE) == 0) {
	    msg->type =   DATA_FRAME;                     // DATA   FRAME
	  }  else  {
	    msg->type = REMOTE_FRAME;                     // REMOTE FRAME
	  }

	  msg->len = (char)0x0000000F & CAN->sFIFOMailBox[0].RDTR;
	                                                    // Read data
	  msg->data[0] = (u_int)0x000000FF & (CAN->sFIFOMailBox[0].RDLR);
	  msg->data[1] = (u_int)0x000000FF & (CAN->sFIFOMailBox[0].RDLR >> 8);
	  msg->data[2] = (u_int)0x000000FF & (CAN->sFIFOMailBox[0].RDLR >> 16);
	  msg->data[3] = (u_int)0x000000FF & (CAN->sFIFOMailBox[0].RDLR >> 24);

	  msg->data[4] = (u_int)0x000000FF & (CAN->sFIFOMailBox[0].RDHR);
	  msg->data[5] = (u_int)0x000000FF & (CAN->sFIFOMailBox[0].RDHR >> 8);
	  msg->data[6] = (u_int)0x000000FF & (CAN->sFIFOMailBox[0].RDHR >> 16);
	  msg->data[7] = (u_int)0x000000FF & (CAN->sFIFOMailBox[0].RDHR >> 24);

	  CAN->RF0R |= CAN_RF0R_RFOM0;                    // Release FIFO 0 output mailbox

}


void Can_Set_Filter(uint id, char format) {

	static unsigned short CAN_filterIdx = 0;
	uint CAN_msgId = 0;

	  if (CAN_filterIdx > 13) {                       // check if Filter Memory is full
	    return;
	  }
	                                                  // Setup identifier information
	  if (format == STANDARD_FORMAT)  {               // Standard ID
	      CAN_msgId  |= (unsigned int)(id << 21) | CAN_ID_STD;
	  }  else  {                                      // Extended ID
	      CAN_msgId  |= (unsigned int)(id <<  3) | CAN_ID_EXT;
	  }

	  CAN->FMR  |=  CAN_FMR_FINIT;                    // set Initialisation mode for filter banks
	  CAN->FA1R &=  ~(unsigned int)(1 << CAN_filterIdx); // deactivate filter

	                                                  // initialize filter
	  CAN->FS1R |= (unsigned int)(1 << CAN_filterIdx);// set 32-bit scale configuration
	  CAN->FM1R |= (unsigned int)(1 << CAN_filterIdx);// set 2 32-bit identifier list mode

	  CAN->sFilterRegister[CAN_filterIdx].FR1 = CAN_msgId; //  32-bit identifier
	  CAN->sFilterRegister[CAN_filterIdx].FR2 = CAN_msgId; //  32-bit identifier

	  CAN->FFA1R &= ~(unsigned int)(1 << CAN_filterIdx);  // assign filter to FIFO 0
	  CAN->FA1R  |=  (unsigned int)(1 << CAN_filterIdx);  // activate filter

	  CAN->FMR &= ~CAN_FMR_FINIT;                     // reset Initialisation mode for filter banks

	  CAN_filterIdx += 1;                             // increase filter index

}


__attribute__((interrupt)) void CAN_TX_IRQHandler (void)  {
	if (CAN->TSR & CAN_TSR_RQCP0) {                 // request completed mbx 0
	    CAN->TSR |= CAN_TSR_RQCP0;                  // reset request complete mbx 0
	    CAN->IER &= ~CAN_IER_TMEIE;                 // disable  TME interrupt
	}

	// Tx Interrupt Action (insert relevant code if needed)

}



__attribute__((interrupt)) void CAN_RX0_IRQHandler (void) {

	if (CAN->RF0R & CAN_RF0R_FMP0) {			      // message pending ?
		Can_Rx_Msg(&can_rx_message);                  // read the message
	}

}



