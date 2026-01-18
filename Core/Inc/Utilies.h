#pragma once
/*
 * Utilies.h
 *
 *  Created on: Oct 12, 2025
 *      Author: yuval yeruham
 */
#include "EEprom.h"
// =================================================================================================
// ========================================= NAMESPACES ============================================

// =================================================================================================
// =========================================== MACROS ==============================================

// =================================================================================================
// ====================================== CONSTANTS, ENUMS =========================================

// =================================================================================================
// ================================ TYPES, CLASSES, STRUCTURES =====================================

// =================================================================================================
// ===================================== FUNCTION PROTOTYPES =======================================

// =================================================================================================
// ===================================== FUNCTION DEFINITIONS ======================================
void Initialize();
void AirSenseControl(GPIO_PinState airSenseStatus_);
void BuzzerControl(GPIO_PinState buzzerStatus_);
void ChangePowerVoltage(E_ENCODER_NUMBER encoder_, E_POWER encoderPower_);
void DipSwitchStatus();
void FrontPanelControl(uint8_t ledNumber_, GPIO_PinState ledStatus_);
void MotorEnableControl(GPIO_PinState MotorStatus_);
void TimerReset(TIM_HandleTypeDef *htim);
void EmptyUartBuffers();
void TriggerToAcsControl(GPIO_PinState state_);
void ResetSynkAndHalt();
void Tp20CheckForError();
