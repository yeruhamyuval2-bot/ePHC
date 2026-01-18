/*
 * communication.h
 *
 *  Created on: Oct 26, 2025
 *      Author: yuval yeruham
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "stm32f4xx_hal.h"
#include "MSGlobals.h"
#include "Types.h"
#include "Utilies.h"
#include <stdbool.h>
#include <stdlib.h>

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
void ReceiveFromCmmController();
void ReceiveFromAcs();
/**
 * @brief Transmit data over UART using DMA
 * @param huart    Pointer to UART handle
 * @param pTxData  Pointer to transmit data buffer
 * @param u8Size   Number of bytes to transmit
 * @return HAL_StatusTypeDef
 *   - HAL_OK:    Transmission started successfully
 *   - HAL_BUSY:  UART peripheral is currently busy
 * @note Function checks UART ready state before initiating DMA transfer
 */
HAL_StatusTypeDef SendData(UART_HandleTypeDef* huart, uint8_t* pTxData, uint8_t u8Size);
void ResetCommunication();
#endif /* COMMUNICATION_H_ */
