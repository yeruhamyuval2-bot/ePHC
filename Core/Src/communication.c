/*
 * communication.c
 *
 *  Created on: Oct 26, 2025
 *      Author: yuval yeruham
 */

#include "communication.h"
#include "cmsis_os.h"

extern UART_HandleTypeDef huart1;
// extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern osThreadId_t TaskUART1Handle;
extern osThreadId_t TaskUART6Handle;
extern osSemaphoreId_t Uart6TxSemHandle;
extern osSemaphoreId_t Uart1TxSemHandle;
/* function prototype */

/* Variables declaration*/
static uint8_t u8XoffFlag = 0;
static uint8_t currentByte;
static uint8_t commandType;

/****************************************************************************
   DESCRIPTION   : Reset all communication parameters
   Parameter in  : NONE
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
static inline void ResetParsing()
{
	u8ReadIdxCmm = 0;
}

/****************************************************************************
   DESCRIPTION   : Interrupt communication RX function
   Parameter in  : UART structure pointer
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// from ACS
	if (huart->Instance == USART6)
	{
		ReceiveFromAcs();
	}

	// From ePH
	else if (huart->Instance == USART2)
	{
	}

	// From CMM controller
	else if (huart->Instance == USART1)
	{
		ReceiveFromCmmController();
	}
}

/****************************************************************************
   DESCRIPTION   : Interrupt communication TX function
   Parameter in  : UART structure pointer
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		osSemaphoreRelease(Uart1TxSemHandle);
	}

	else if (huart->Instance == USART6)
	{
		osSemaphoreRelease(Uart6TxSemHandle);
	}
}

/****************************************************************************
   DESCRIPTION   : Get data from ACS ECMSM
   Parameter in  : NONE
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void ReceiveFromAcs()
{
	uint8_t currentByte = au8RxBufAcs.uaiDataArray[u8ReadIdxAcs];
	if (u8ReadIdxAcs >= RENISHAW_CMD_MAX_LENGTH)
	{
		osThreadFlagsSet(TaskUART6Handle, EVENT_ERROR);
		return;
	}

	switch (eReturnCodeState)
	{
	case IDLE:
		if (currentByte == RENISHAW_XOFF_CMD)
		{
			u8XoffFlag = 1;
			au8RxBufAcs.uSize = ++u8ReadIdxAcs;
			u8ReadIdxAcs = 0;
			TimerReset(&htim11);
			HAL_TIM_Base_Start_IT(&htim11); // 5 second in case we are moving the head
			osThreadFlagsSet(TaskUART1Handle, EVENT_DATA_READY);
		}

		else
		{
			eReturnCodeState = WAIT_FOR_CR;
			if (u8XoffFlag)
				HAL_TIM_Base_Stop_IT(&htim11);
			TimerReset(&htim10);
			HAL_TIM_Base_Start_IT(&htim10); // 0.1 second
			HAL_UART_Receive_IT(&huart6, (uint8_t *)&au8RxBufAcs.uaiDataArray[++u8ReadIdxAcs], 1);
		}
		break;

	case WAIT_FOR_CR:
		if (currentByte == RENISHAW_CR_CMD)
		{
			HAL_TIM_Base_Stop_IT(&htim10);
			au8RxBufAcs.uSize = ++u8ReadIdxAcs;
			u8ReadIdxAcs = 0;
			if (u8XoffFlag)
			{
				TimerReset(&htim10);
				HAL_TIM_Base_Start_IT(&htim10); // 0.1 second
				eReturnCodeState = WAIT_FOR_XON;
				u8XoffFlag = 0;
			}
			else
			{
				eReturnCodeState = IDLE;
			}
			osThreadFlagsSet(TaskUART1Handle, EVENT_DATA_READY);
		}
		else
			HAL_UART_Receive_IT(&huart6, (uint8_t *)&au8RxBufAcs.uaiDataArray[++u8ReadIdxAcs], 1);

		break;

	case WAIT_FOR_XON:
		HAL_TIM_Base_Stop_IT(&htim10);
		au8RxBufAcs.uSize = ++u8ReadIdxAcs;
		u8ReadIdxAcs = 0;
		eReturnCodeState = IDLE;
		osThreadFlagsSet(TaskUART1Handle, EVENT_DATA_READY);
		break;

	default:
		osThreadFlagsSet(TaskUART1Handle, EVENT_ERROR);
		break;
	}
}

/****************************************************************************
   DESCRIPTION   : Get data from CMM controller
   Parameter in  : NONE
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void ReceiveFromCmmController()
{
	/* FIX: CRITICAL - Check bounds BEFORE array access (same as ReceiveFromAcs) */
	if (u8ReadIdxCmm >= RENISHAW_CMD_MAX_LENGTH)
	{
		u8ReadIdxCmm = 0; /* Reset index */
		osThreadFlagsSet(TaskUART6Handle, EVENT_ERROR);
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&au8RxBufCmm.uaiDataArray[0], 1);
		return;
	}

	if (u8ReadIdxCmm == 0)
	{
		TimerReset(&htim10);
		HAL_TIM_Base_Start_IT(&htim10); // 0.1 second
	}

	currentByte = au8RxBufCmm.uaiDataArray[u8ReadIdxCmm];
	commandType = au8RxBufCmm.uaiDataArray[0];

	switch (commandType)
	{
	case RENISHAW_A_CMD:
	case RENISHAW_B_CMD:
	case RENISHAW_M_CMD:
	case RENISHAW_N_CMD:
	case RENISHAW_S_CMD:
	case RENISHAW_U_CMD:
	case RENISHAW_K_CMD:
	case RENISHAW_Q_CMD:
	case RENISHAW_R_CMD:
		if (currentByte == RENISHAW_CR_CMD)
		{
			u8ParsingDone = 1;
		}
		break;

	default:
	{
		/* FIX: Reset on invalid command instead of silent failure */
		u8ReadIdxCmm = 0;  /* Clear index */
		u8ParsingDone = 0; /* Reset parsing flag */
		osThreadFlagsSet(TaskUART6Handle, EVENT_ERROR);
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&au8RxBufCmm.uaiDataArray[0], 1);
		return;
	}
	}

	if (u8ParsingDone)
	{
		u8ParsingDone = 0;
		au8RxBufCmm.uSize = ++u8ReadIdxCmm;
		ResetParsing();
		osThreadFlagsSet(TaskUART6Handle, EVENT_DATA_READY);
	}
	else
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&au8RxBufCmm.uaiDataArray[++u8ReadIdxCmm], 1);
}

/**
 * @brief Transmit data over UART using DMA
 * @details This function checks if the UART peripheral is in a ready state before
 *          initiating a DMA transfer. If the UART is busy, it returns HAL_BUSY status.
 * @param huart    Pointer to UART handle structure
 * @param pTxData  Pointer to the transmit data buffer
 * @param u8Size   Number of bytes to transmit
 * @return HAL_StatusTypeDef
 *   - HAL_OK:    DMA transmission started successfully
 *   - HAL_BUSY:  UART peripheral is currently busy with another operation
 * @note Function is interrupt-driven; use semaphore or callback to detect completion
 */
HAL_StatusTypeDef SendData(UART_HandleTypeDef *huart, uint8_t *pTxData, uint8_t u8Size)
{
	if (huart->gState == HAL_UART_STATE_READY)
	{
		return HAL_UART_Transmit_DMA(huart, pTxData, u8Size);
	}
	else
	{
		return HAL_BUSY;
	}
}

/****************************************************************************
   DESCRIPTION   : Reset UART communication
   Parameter in  : NONE
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void ResetCommunication()
{
	HAL_UART_AbortTransmit(&huart1);
	HAL_UART_AbortTransmit(&huart6);
	u8ReadIdxCmm = 0;
	u8ReadIdxAcs = 0;
	eReturnCodeState = IDLE;
	HAL_UART_Receive(&huart1, (uint8_t *)au8RxBufCmm.uaiDataArray, RENISHAW_CMD_MAX_LENGTH, 100);
	HAL_UART_Receive(&huart6, (uint8_t *)au8RxBufAcs.uaiDataArray, RENISHAW_CMD_MAX_LENGTH, 100);
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&au8RxBufCmm.uaiDataArray[0], 1);
	HAL_UART_Receive_IT(&huart6, (uint8_t *)&au8RxBufAcs.uaiDataArray[0], 1);

	return;
}
