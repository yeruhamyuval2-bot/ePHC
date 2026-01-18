/*
 * Types.h
 *
 *  Created on: Oct 16, 2025
 *      Author: yuval yeruham
 */

#ifndef INC_TYPES_H_
#define INC_TYPES_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"

#pragma pack (push, 1)

typedef struct
{
	GPIO_TypeDef* Port;
	uint16_t PinNumber;
}sPinConfig;

#pragma pack(pop)

//ACS messages structures
typedef struct
{
	uint8_t 	uaiDataArray[RENISHAW_CMD_MAX_LENGTH];
	uint8_t 	uSize;
} sCommunicationReceive;

// -----------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* INC_TYPES_H_ */
