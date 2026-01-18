/*
 * MSGlobals.h
 *
 *  Created on: Oct 15, 2025
 *      Author: yuval yeruham
 */

#ifndef INC_MSGLOBALS_H_
#define INC_MSGLOBALS_H_

#include "Constant.h"
#include "Types.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef  extt
  #define  extt extern
#endif

typedef enum
{
	V5_0,
	V5_6,
	V6_1,
	V6_6
}E_POWER;

typedef enum
{
	A,
	B,
	HUB
}E_ENCODER_NUMBER;

enum COMMAND_PROCESS_TYPE
{
	IDLE_STATE = 0,
	READ_STATE = 1,
	SEND_STATE = 2,
	RESET_UART = 0xFF
};

enum RENISHAW_COMMAND
{
	RENISHAW_A_CMD     = 'A',
	RENISHAW_B_CMD     = 'B',
	RENISHAW_S_CMD     = 'S',
	RENISHAW_U_CMD     = 'U',
	RENISHAW_M_CMD     = 'M',
	RENISHAW_N_CMD     = 'N',

	RENISHAW_C_CMD     = 'C',
	RENISHAW_E_CMD     = 'E',
	RENISHAW_F_CMD     = 'F',
	RENISHAW_H_CMD     = 'H',
	RENISHAW_I_CMD     = 'I',
	RENISHAW_J_CMD     = 'J',
	RENISHAW_O_CMD     = 'O',
	RENISHAW_T_CMD     = 'T',
	RENISHAW_V_CMD     = 'V',
	RENISHAW_X_CMD     = 'X',
	RENISHAW_K_CMD     = 'K',
	RENISHAW_Q_CMD     = 'Q',
	RENISHAW_R_CMD     = 'R',
	RENISHAW_XON_CMD   = 17,
	RENISHAW_XOFF_CMD  = 19,
	RENISHAW_CR_CMD    = '\r'
};

enum RENISHAW_RC_STATES
{
	IDLE,
	WAIT_FOR_XON,
	WAIT_FOR_CR,
};

//GPIOs
extt sPinConfig sPowerPin[3][3];
extt sPinConfig sDipSwitchesPin[12];
extt sPinConfig sLedError[7];
/** @brief DIP switch status register - holds state of 12 DIP switches (bits 0-11). Changed from uint8_t to uint16_t to prevent bit shift overflow */
extt uint16_t 	u16DipSwitchesStatus;
extt uint8_t 	u8Tp20ErrorDetect;

// Globals variables
extt volatile uint8_t 		u8ReadIdxCmm;
extt volatile uint8_t	 	u8ReadIdxAcs;
extt volatile uint8_t 		u8ParsingDone;
extt volatile GPIO_PinState gpPdumpStatus;
extt volatile GPIO_PinState gpPpoffStatus;
extt volatile uint8_t 		u8IsFirstHaltArrived;
extt volatile uint8_t 		u8IsFirstSnycArrived;
extt volatile uint8_t 		u8IsInTp20Measurement;
extt volatile uint16_t 		u161MilliSecondCounter;

// UART Globals buffers
extt volatile sCommunicationReceive au8RxBufAcs;
extt sCommunicationReceive au8TxBufAcs;
extt volatile sCommunicationReceive au8RxBufEph;
extt sCommunicationReceive au8TxBufEph;
extt volatile sCommunicationReceive au8RxBufCmm;
extt sCommunicationReceive au8TxBufCmm;

// SPI #2 Global buffers
extt uint8_t au8RxBufferSPI2[SPI_2_BUFF_LEN];    // Read data from SPI2 EEPROM
extt uint8_t au8TxBufferSPI2[SPI_2_BUFF_LEN];    // Write data to SPI2 EEPROM
extt uint8_t au8CmdDataSize[SPI_2_BUFF_LEN];

//state machine variables
extt volatile enum RENISHAW_RC_STATES eReturnCodeState;
extt volatile enum COMMAND_PROCESS_TYPE eTask1State;
extt volatile enum COMMAND_PROCESS_TYPE eTask2State;


#endif /* INC_MSGLOBALS_H_ */
