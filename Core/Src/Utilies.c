/*
 * Utilies.c
 *
 *  Created on: Oct 12, 2025
 *      Author: yuval yeruham
 */
#include "Utilies.h"
#include <MSGlobals.h>

extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;

/****************************************************************************
   DESCRIPTION   : Variables initialize
   Parameter in  : On/Off
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void Initialize()
{
	sPowerPin[0][0].Port = ENCA_CV1_GPIO_Port;
	sPowerPin[0][0].PinNumber = ENCA_CV1_Pin;
	sPowerPin[0][1].Port = ENCA_CV2_GPIO_Port;
	sPowerPin[0][1].PinNumber = ENCA_CV2_Pin;
	sPowerPin[0][2].Port = ENCA_CV3_GPIO_Port;
	sPowerPin[0][2].PinNumber = ENCA_CV3_Pin;

	sPowerPin[1][0].Port = ENCB_CV1_GPIO_Port;
	sPowerPin[1][0].PinNumber = ENCB_CV1_Pin;
	sPowerPin[1][1].Port = ENCB_CV2_GPIO_Port;
	sPowerPin[1][1].PinNumber = ENCB_CV2_Pin;
	sPowerPin[1][2].Port = ENCB_CV3_GPIO_Port;
	sPowerPin[1][2].PinNumber = ENCB_CV3_Pin;

	sPowerPin[2][0].Port = HUB_CV1_GPIO_Port;
	sPowerPin[2][0].PinNumber = HUB_CV1_Pin;
	sPowerPin[2][1].Port = HUB_CV2_GPIO_Port;
	sPowerPin[2][1].PinNumber = HUB_CV2_Pin;
	sPowerPin[2][2].Port = HUB_CV3_GPIO_Port;
	sPowerPin[2][2].PinNumber = HUB_CV3_Pin;

	sDipSwitchesPin[0].Port = DIP_SW1_GPIO_Port;
	sDipSwitchesPin[0].PinNumber = DIP_SW1_Pin;
	sDipSwitchesPin[1].Port = DIP_SW2_GPIO_Port;
	sDipSwitchesPin[1].PinNumber = DIP_SW2_Pin;
	sDipSwitchesPin[2].Port = DIP_SW3_GPIO_Port;
	sDipSwitchesPin[2].PinNumber = DIP_SW3_Pin;
	sDipSwitchesPin[3].Port = DIP_SW4_GPIO_Port;
	sDipSwitchesPin[3].PinNumber = DIP_SW4_Pin;
	sDipSwitchesPin[4].Port = DIP_SW5_GPIO_Port;
	sDipSwitchesPin[4].PinNumber = DIP_SW5_Pin;
	sDipSwitchesPin[5].Port = DIP_SW6_GPIO_Port;
	sDipSwitchesPin[5].PinNumber = DIP_SW6_Pin;
	sDipSwitchesPin[6].Port = DIP_SW7_GPIO_Port;
	sDipSwitchesPin[6].PinNumber = DIP_SW7_Pin;
	sDipSwitchesPin[7].Port = DIP_SW8_GPIO_Port;
	sDipSwitchesPin[7].PinNumber = DIP_SW8_Pin;
	sDipSwitchesPin[8].Port = DIP_SW9_GPIO_Port;
	sDipSwitchesPin[8].PinNumber = DIP_SW9_Pin;
	sDipSwitchesPin[9].Port = DIP_SW10_GPIO_Port;
	sDipSwitchesPin[9].PinNumber = DIP_SW10_Pin;
	sDipSwitchesPin[10].Port = DIP_SW11_GPIO_Port;
	sDipSwitchesPin[10].PinNumber = DIP_SW11_Pin;
	sDipSwitchesPin[11].Port = DIP_SW12_GPIO_Port;
	sDipSwitchesPin[11].PinNumber = DIP_SW12_Pin;

	sLedError[0].Port = FRONT_POWER_LED_GPIO_Port;
	sLedError[0].PinNumber = FRONT_POWER_LED_Pin;
	sLedError[1].Port = FRONT_STOP_LED_GPIO_Port;
	sLedError[1].PinNumber = FRONT_STOP_LED_Pin;
	sLedError[2].Port = FRONT_READY_LED_GPIO_Port;
	sLedError[2].PinNumber = FRONT_READY_LED_Pin;
	sLedError[3].Port = FRONT_ACTIVE_LED_GPIO_Port;
	sLedError[3].PinNumber = FRONT_ACTIVE_LED_Pin;
	sLedError[4].Port = FRONT_ERROR1_LED_GPIO_Port;
	sLedError[4].PinNumber = FRONT_ERROR1_LED_Pin;
	sLedError[5].Port = FRONT_ERROR2_LED_GPIO_Port;
	sLedError[5].PinNumber = FRONT_ERROR2_LED_Pin;
	sLedError[6].Port = FRONT_ERROR3_LED_GPIO_Port;
	sLedError[6].PinNumber = FRONT_ERROR3_LED_Pin;

	eReturnCodeState = IDLE;
	eTask1State = IDLE_STATE;
	eTask2State = IDLE_STATE;

	EmptyUartBuffers();

	u8ReadIdxCmm = 0;
	u8ReadIdxAcs = 0;

	HAL_TIM_Base_Start_IT(&htim9); // running timer for counting 1 millisecond each interrupt
	MotorEnableControl(GPIO_PIN_SET);
	HAL_GPIO_WritePin(AIR_SENSE_CNT_GPIO_Port, AIR_SENSE_CNT_Pin, GPIO_PIN_SET);
}

/****************************************************************************
   DESCRIPTION   : Set all buffers data to 0
   Parameter in  : NONE
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void EmptyUartBuffers()
{
	memset((uint8_t *)&au8RxBufAcs, 0, RENISHAW_CMD_MAX_LENGTH + 1);
	memset((uint8_t *)&au8TxBufAcs, 0, RENISHAW_CMD_MAX_LENGTH + 1);
	memset((uint8_t *)&au8RxBufEph, 0, RENISHAW_CMD_MAX_LENGTH + 1);
	memset((uint8_t *)&au8TxBufEph, 0, RENISHAW_CMD_MAX_LENGTH + 1);
	memset((uint8_t *)&au8RxBufCmm, 0, RENISHAW_CMD_MAX_LENGTH + 1);
	memset((uint8_t *)&au8TxBufCmm, 0, RENISHAW_CMD_MAX_LENGTH + 1);
	return;
}

/****************************************************************************
   DESCRIPTION   : Buzzer Control
   Parameter in  : On/Off
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void BuzzerControl(GPIO_PinState buzzerStatus_)
{
	HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, buzzerStatus_);
	return;
}

/****************************************************************************
   DESCRIPTION   : Air sense Control
   Parameter in  : Open/Close
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void AirSenseControl(GPIO_PinState airSenseStatus_)
{
	HAL_GPIO_WritePin(AIR_SENSE_CNT_GPIO_Port, AIR_SENSE_CNT_Pin, airSenseStatus_);
	return;
}

/****************************************************************************
   DESCRIPTION   : Change encoder power supply (6.6, 6.1, 5.6)V
   Parameter in  : state (CV1 = 5.6, CV2 = 6.1, CV3 = 6.6)
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void ChangePowerVoltage(E_ENCODER_NUMBER encoder_, E_POWER encoderPower_)
{
	switch (encoderPower_)
	{
	case V5_0:
		HAL_GPIO_WritePin(sPowerPin[encoder_][0].Port, sPowerPin[encoder_][0].PinNumber, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(sPowerPin[encoder_][1].Port, sPowerPin[encoder_][1].PinNumber, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(sPowerPin[encoder_][2].Port, sPowerPin[encoder_][2].PinNumber, GPIO_PIN_RESET);
		break;
	case V5_6:
		HAL_GPIO_WritePin(sPowerPin[encoder_][0].Port, sPowerPin[encoder_][0].PinNumber, GPIO_PIN_SET);
		HAL_GPIO_WritePin(sPowerPin[encoder_][1].Port, sPowerPin[encoder_][1].PinNumber, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(sPowerPin[encoder_][2].Port, sPowerPin[encoder_][2].PinNumber, GPIO_PIN_RESET);
		break;
	case V6_1:
		HAL_GPIO_WritePin(sPowerPin[encoder_][0].Port, sPowerPin[encoder_][0].PinNumber, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(sPowerPin[encoder_][1].Port, sPowerPin[encoder_][1].PinNumber, GPIO_PIN_SET);
		HAL_GPIO_WritePin(sPowerPin[encoder_][2].Port, sPowerPin[encoder_][2].PinNumber, GPIO_PIN_RESET);
		break;
	case V6_6:
		HAL_GPIO_WritePin(sPowerPin[encoder_][0].Port, sPowerPin[encoder_][0].PinNumber, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(sPowerPin[encoder_][1].Port, sPowerPin[encoder_][1].PinNumber, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(sPowerPin[encoder_][2].Port, sPowerPin[encoder_][2].PinNumber, GPIO_PIN_SET);
		break;

	default:
		break;
	}
	return;
}

/**
 * @brief Read and update the status of all 12 DIP switches
 * @details Iterates through each DIP switch GPIO pin and reads its state.
 *          The result is stored in a temporary variable FIRST, then atomically
 *          written to u16DipSwitchesStatus with mutex protection to prevent race conditions.
 *          NULL port checks prevent reading uninitialized GPIO pins.
 * @return void
 * @note Each switch occupies one bit (0-11) in u16DipSwitchesStatus register
 * @note CRITICAL FIX: Temporary variable prevents accumulation of old bit values
 * @note CRITICAL FIX: Mutex protection ensures atomic update from main task level
 */
void DipSwitchStatus()
{
	/* FIX: Use temporary variable to accumulate new state - prevents bit accumulation */
	uint16_t u16NewStatus = 0;

	for (int i = 0; i < 12; i++)
	{
		/* FIX: NULL check prevents GPIO driver errors if pin is uninitialized */
		if (sDipSwitchesPin[i].Port != NULL)
		{
			/* FIX: Cast to uint16_t required to safely shift GPIO bit to position i (max 11)
			   Without cast, shifting uint8_t by 8+ bits produces undefined C behavior */
			u16NewStatus |= (uint16_t)HAL_GPIO_ReadPin(sDipSwitchesPin[i].Port, sDipSwitchesPin[i].PinNumber) << i;
		}
	}

	/* FIX: CRITICAL - Use mutex to protect atomic write to global variable
	   Prevents race condition between this task and interrupt handlers or other tasks */
	if (osMutexAcquire(SignalMutexHandle, 100) == osOK)
	{
		u16DipSwitchesStatus = u16NewStatus;
		osMutexRelease(SignalMutexHandle);
	}
	/* Note: If mutex acquire fails (timeout), skip update to maintain last valid state */

	return;
}

/****************************************************************************
   DESCRIPTION   : Turn On/Off front panel LEDs
   Parameter in  : LED number
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void FrontPanelControl(uint8_t ledNumber_, GPIO_PinState ledStatus_)
{
	HAL_GPIO_WritePin(sLedError[ledNumber_].Port, sLedError[ledNumber_].PinNumber, ledStatus_);
	return;
}

/****************************************************************************
   DESCRIPTION   : Motor-Enable 24 volt control
   Parameter in  : enable - supply 24V to engines, disable - do not supply power to the engines
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void MotorEnableControl(GPIO_PinState MotorStatus_)
{
	HAL_GPIO_WritePin(MOTO_EN_24V_GPIO_Port, MOTO_EN_24V_Pin, MotorStatus_);
	return;
}

/****************************************************************************
   DESCRIPTION   : Reset timer counter to 0
   Parameter in  : timer pointer handler
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void TimerReset(TIM_HandleTypeDef *htim)
{
	__HAL_TIM_SET_COUNTER(htim, 0);
	__HAL_TIM_CLEAR_FLAG(htim, TIM_SR_UIF);
	__HAL_TIM_CLEAR_FLAG(htim, TIM_IT_UPDATE);
	return;
}

/****************************************************************************
   DESCRIPTION   : Set Mark0 trigger to ACS
   Parameter in  : timer pointer handler
   Returns       : NONE
   Parameter out : NONE
   Globals       :
****************************************************************************/
void TriggerToAcsControl(GPIO_PinState state_)
{
	HAL_GPIO_WritePin(MARK0_GPIO_Port, MARK0_Pin, state_);
	TimerReset(&htim5);
	HAL_TIM_Base_Start_IT(&htim5);
	return;
}

/**
 * @brief Reset Renishaw SYNC/HALT protocol flags to initial state
 * @param None
 * @return None
 * @note CRITICAL: This function modifies shared state protected by SignalMutexHandle.
 *       Caller MUST hold the mutex before calling this function.
 *       Do NOT call directly without mutex - call through SignalMutexHandle protected section.
 * @warning If called from interrupt context, may cause deadlock. ISRs should signal
 *          a task to perform the reset within a mutex-protected section.
 */
void ResetSynkAndHalt()
{
	/* FIX: Caller MUST hold SignalMutexHandle before calling this function
	   These variables are shared between TaskSYNC (RealTime7) and TaskHALT (RealTime7)
	   without mutex would cause race conditions */
	u8IsInTp20Measurement = 0;
	u161MilliSecondCounter = 0;
	u8IsFirstHaltArrived = FALSE;
	u8IsFirstSnycArrived = FALSE;
	return;
}

// =================================================================================================
// Check if tp20 is disconnected
//
// Author: Michael, Date: November 25, 2021
// -------------------------------------------------------------------------------------------------
void Tp20CheckForError()
{
	if ((!u8IsInTp20Measurement) && (HAL_GPIO_ReadPin(MARK1_SYNC_GPIO_Port, MARK1_SYNC_Pin) == IN_SNYC_ON) && (HAL_GPIO_ReadPin(MARK2_HALT_GPIO_Port, MARK2_HALT_Pin) == IN_HALT_ON))
	{
		u8Tp20ErrorDetect = 1;
		// TODO: currently there is no IO for that action
	}
	else if ((u8Tp20ErrorDetect) && (HAL_GPIO_ReadPin(MARK1_SYNC_GPIO_Port, MARK1_SYNC_Pin) == IN_SNYC_OFF) && (HAL_GPIO_ReadPin(MARK2_HALT_GPIO_Port, MARK2_HALT_Pin) == IN_HALT_OFF))
	{
		u8Tp20ErrorDetect = 0;
		// TODO: currently there is no IO for that action
	}
	return;
}
