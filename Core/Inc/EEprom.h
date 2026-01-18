/*
 * EEprom.h
 *
 *  Created on: Oct 19, 2025
 *      Author: yuval yeruham
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "main.h"
//-------------------------------------------------------------------------------------------------------
// Global Constants
//-------------------------------------------------------------------------------------------------------

#define  SPI_WRITE         0x04           // Send a byte from the Master to the Slave
#define  SPI_READ          0x08           // Send a byte from the Slave to the Master
#define  SPI_READ_WRITE    0x30           // Send command Read a byte

#define  ERROR_OCCURRED    0x40           // Indicator for the Slave to tell the

// Configure of SPI channel
#define SPI_CLOCK_04         400000L      // Maximum SPI clock is a maximum of 400 kHz
#define SPI_CLOCK_1         1000000L      // Maximum SPI clock is a maximum of 1 MHz
#define SPI_CLOCK_2         2000000L      // Maximum SPI clock is a maximum of 2 MHz
#define CKPHA_0         0x00000000b       // CKPHA = 0
#define CKPHA_1         0x00100000b       // CKPHA = 1
#define CKPOL_0         0x00000000b       // CKPOL = 0
#define CKPOL_1         0x00010000b       // CKPOL = 1

#define CKPHA_0_CKPOL_0         0x40      // SPI as a Master  CKPOL=0, CKPHA=0
#define CKPHA_0_CKPOL_1         0x50      // SPI as a Master  CKPOL=1, CKPHA=0
#define CKPHA_1_CKPOL_0         0x60      // SPI as a Master  CKPOL=0, CKPHA=1
#define CKPHA_1_CKPOL_1         0x70      // SPI as a Master  CKPOL=1, CKPHA=1

//  **********************    E2  M95640  CONSTANTS   **********************
#define   WREN                     0x06                      // 0000 0110 Enable Write Operations
#define   WRDI                     0x04                      // 0000 0100 Disable Write Operations
#define   RDSR                     0x05                      // 0000 0101 Read Status Register
#define   WRSR                     0x01                      // 0000 0001 Write Status Register
#define   READ                     0x03                      // 0000 0011 Read Data from Memory
#define   WRITE                    0x02                      // 0000 0010 Write Data to Memory

#define   E2_BLOCK_SIZE               4                      // Block size to write

#define   E2_PARAM_ADD_1           0x100                     // Parameters address
#define   E2_PARAM_ADD_2           0x200                     // Parameters address
#define   E2_HARD_VER_ADD          0x400                     // Calibration Parameters address

/*---------------------------------------------------------------------------*/
/*     Global functions                                                      */
/*---------------------------------------------------------------------------*/
HAL_StatusTypeDef EEPROM_Set_Write_Enable (void) ;
HAL_StatusTypeDef EEPROM_Read_Data (unsigned int address, unsigned char * buffer, short length);
HAL_StatusTypeDef EEPROM_Write_Data (unsigned int address, unsigned char * buffer, short length) ;

#endif /* EEPROM_H_ */
