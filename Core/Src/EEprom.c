/****************************************************************************
*
 * EEprom.c
 *
 *  Created on: Oct 19, 2025
 *      Author: yuval yeruham
 *

****************************************************************************/

/*---------------------------------------------------------------------------*/
/* Local types                                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Local variables                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Global functions                                                          */
/*---------------------------------------------------------------------------*/
#include "EEprom.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//-------------------------------------------------------------------------------------------------------
// Global Constants
//-------------------------------------------------------------------------------------------------------

extern  SPI_HandleTypeDef hspi2;

/********************************************************************************************************************
   DESCRIPTION  : Set Write disable to E2
   Parameter in : Non.
   Parameter out:
   Returns      :
   Globals      : g_cTxBuffer.
   Execution time : 25uS
********************************************************************************************************************/
HAL_StatusTypeDef EEPROM_Set_Write_Disable (void)
{
	HAL_StatusTypeDef EE_ststus;

   HAL_GPIO_WritePin(CS_EE2_GPIO_Port, CS_EE2_Pin, GPIO_PIN_RESET);
   au8TxBufferSPI2[0] = WRDI ;                  // Set the write enable latch (enable write operations)
   EE_ststus = HAL_SPI_TransmitReceive(&hspi2, au8TxBufferSPI2, au8RxBufferSPI2, 1, 100) ;
   HAL_GPIO_WritePin(CS_EE2_GPIO_Port, CS_EE2_Pin, GPIO_PIN_SET);
   if (EE_ststus != HAL_OK)
      __NOP();            // For test only
   return EE_ststus;
}

/********************************************************************************************************************
   DESCRIPTION  : Set Write enable to E2
   Parameter in : Non.
   Parameter out:
   Returns      :
   Globals      : g_cTxBuffer.
   Execution time : 25uS
********************************************************************************************************************/
HAL_StatusTypeDef EEPROM_Set_Write_Enable (void)
{
	HAL_StatusTypeDef EE_ststus;

   HAL_GPIO_WritePin(CS_EE2_GPIO_Port, CS_EE2_Pin, GPIO_PIN_RESET);
   au8TxBufferSPI2[0] = WREN ;                  // Set the write enable latch (enable write operations)
   EE_ststus = HAL_SPI_TransmitReceive(&hspi2, au8TxBufferSPI2, au8RxBufferSPI2, 1, 100) ;
   HAL_GPIO_WritePin(CS_EE2_GPIO_Port, CS_EE2_Pin, GPIO_PIN_SET);
   if (EE_ststus != HAL_OK)
      __NOP();            // For test only
   return EE_ststus;
}


/********************************************************************************************************************
   DESCRIPTION  :
                :
   Parameter in : Non.
   Parameter out:
   Returns      :
   Globals      : au8TxBufferSPI2
   Execution time :
********************************************************************************************************************/
HAL_StatusTypeDef EEPROM_Read_Data (unsigned int address, unsigned char * buffer, short length)
{
	HAL_StatusTypeDef EE_ststus;

   HAL_GPIO_WritePin(CS_EE2_GPIO_Port, CS_EE2_Pin, GPIO_PIN_RESET);
   au8TxBufferSPI2[0] = READ ;                                    // Read command
   au8TxBufferSPI2[1] = (address >> 8) ;                             // Address high byte
   au8TxBufferSPI2[2] = (unsigned char)address  ;                    // Address low byte
   EE_ststus = HAL_SPI_TransmitReceive(&hspi2, au8TxBufferSPI2, au8RxBufferSPI2, length + 3, 100) ;
   memcpy (buffer, &au8RxBufferSPI2[3], length);            // Data
   HAL_GPIO_WritePin(CS_EE2_GPIO_Port, CS_EE2_Pin, GPIO_PIN_SET);    //  E2 UnSelet
   return EE_ststus;
}

/********************************************************************************************************************
   DESCRIPTION  :
                :
   Parameter in : Non.
   Parameter out:
   Returns      :
   Globals      : au8TxBufferSPI2, au8RxBufferSPI2
   Execution time :
********************************************************************************************************************/
HAL_StatusTypeDef EEPROM_Write_Data (unsigned int address, unsigned char * buffer, short length)
{
	HAL_StatusTypeDef EE_ststus;

	short rest, bloks, bloksCount ;
	unsigned int LocalAddress ;

   bloks = length / E2_BLOCK_SIZE;
   rest = length % E2_BLOCK_SIZE ;

   EE_ststus = EEPROM_Set_Write_Enable ();                          // Enable write
   __NOP();
   if (EE_ststus != HAL_OK)      return EE_ststus;

   HAL_GPIO_WritePin(CS_EE2_GPIO_Port, CS_EE2_Pin, GPIO_PIN_RESET);
   if (bloks > 0)
   {                // More than 1 block
      for (bloksCount = 0 ; bloksCount < bloks ; bloksCount++)
      {
         LocalAddress = address + (bloksCount * E2_BLOCK_SIZE) ;
         au8TxBufferSPI2[0] = WRITE ;                             // Write command
         au8TxBufferSPI2[1] = (LocalAddress >> 8) ;               // Address high byte
         au8TxBufferSPI2[2] = (unsigned char)LocalAddress  ;      // Address low byte

         memcpy (&au8TxBufferSPI2[3], buffer + (bloksCount * E2_BLOCK_SIZE), E2_BLOCK_SIZE);            // Data
         EE_ststus = HAL_SPI_Transmit(&hspi2, au8TxBufferSPI2, E2_BLOCK_SIZE + 3, 100) ;
         if (EE_ststus != HAL_OK)   return EE_ststus;
         __NOP();
      }  // for .....
      bloks = 0 ;             // Exit if (bloks > 0)

      if (rest > 0)
      {                 // Remain of data to write
         LocalAddress = address + (bloksCount * E2_BLOCK_SIZE) ;
         au8TxBufferSPI2[0] = WRITE ;                             // Write command
         au8TxBufferSPI2[1] = (LocalAddress >> 8) ;               // Address high byte
         au8TxBufferSPI2[2] = (unsigned char)LocalAddress  ;      // Address low byte

         memcpy (&au8TxBufferSPI2[3], buffer + (bloksCount * E2_BLOCK_SIZE), E2_BLOCK_SIZE);            // Data
         EE_ststus = HAL_SPI_Transmit(&hspi2, au8TxBufferSPI2, E2_BLOCK_SIZE + 3, 100) ;
         if (EE_ststus != HAL_OK)   return EE_ststus;
         __NOP();
      }
   }  // if (bloks > 0)
   else
   {                         // 1 block only
      au8TxBufferSPI2[0] = WRITE ;                             // Write command
      au8TxBufferSPI2[1] = (address >> 8) ;                    // Address high byte
      au8TxBufferSPI2[2] = (unsigned char)address  ;           // Address low byte
      memcpy (&au8TxBufferSPI2[3], buffer, length);            // Data
      EE_ststus = HAL_SPI_Transmit(&hspi2, au8TxBufferSPI2, length + 3, 100) ;
      if (EE_ststus != HAL_OK)   return EE_ststus;
   }

   HAL_GPIO_WritePin(CS_EE2_GPIO_Port, CS_EE2_Pin, GPIO_PIN_SET);
   if (EE_ststus != HAL_OK)
     return EE_ststus;

   EE_ststus = EEPROM_Set_Write_Disable ();               // Disable E2 Write
   return EE_ststus;
}
