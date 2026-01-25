/****************************************************************************
  FILE        : Constant.h
  PROJECT     : Globus Touch Project

  DATE CREATED: Jan 2021
  AUTHOR      : Didi Avram

  DESCRIPTION : Constants files.
              :
  NOTES       :

 * REVISION HISTORY:
 *			Name		  Date		           Description
 *			----		  ----		           -----------
 *			AR			  11/01/21		       Initial Revision

****************************************************************************/

#ifndef _CONSTANT_H
#define _CONSTANT_H

#define ON 1
#define OFF 0
#define TRUE 1
#define FALSE 0
#define PASS 1
#define FAILED 0
#define CHANGE_STAGE 1
#define NOT_CHANGE_STAGE 0
#define FOREVER 1
#define YES ON
#define NO OFF
#define START 1
#define RUN 1
#define STOP 0
#define PASS 1
#define FAIL 0
#define DIRECT 1  // Direct convention
#define INVERSE 0 // Inverse convention
#define TOG 2     // Toggle mode
#define xAxis 0
#define yAxis 1
#define zAxis 2
#define ID_RESISTOR 3

// Renishaw
#define PDUMP 8
#define PPOFF 5
#define HALT 15
#define SYNC 14
#define SNYK_WAIT_TIME 3
#define HALT_TIME_OFF 500
#define SNYK_HALT_TIME_FINISH 1000

//  ************           Pics                 ************
#define IN_PDAMP_ON 1
#define IN_PDAMP_OFF 0
#define IN_SNYC_OFF 1
#define IN_HALT_OFF 1
#define IN_SNYC_ON 0
#define IN_HALT_ON 0

#define CONNECT 1    // Connect command to Mux
#define DISCONNECT 0 // Disconnect command to Mux

#define FROM_CMD_ 3 // Schedule status from command
#define NOTUSED_ 2  // Schedule status Not in use
#define ENABLE_ 1   // Schedule status Enable
#define DISABLE_ 0  // Schedule status Disable

#define BUSSY 1 // Channel bussy - one transmitter activated
#define FREE 0  // Channel free - can connect receivers

#define SPI_BUSY 0 // SPI channel Busy
#define SPI_FREE 1 // SPI channel Not Busy
#define HIGH 1
#define LOW 0

#define ACK 0x80
#define ERROR_CMD 0xFF

#define STX 0x02 // Start of Header
#define ETX 0x03 // End of Header
#define EQ 0x3D  //  '=' character.

#define HSSB1 0
#define HSSB2 1

//  ************           Cmmunication  flags                 ************
#define NO_ERROR 0   // No communication error
#define STX_ERROR 2  // STX communication error
#define LEN_ERROR 3  // Buffer length communication error
#define CSUM_ERROR 4 // Checksum communication error

#define XOFF 19 // DC3
#define XON 17  // DC1

#define OK 0
#define NOT_OK 1
#define NOT_INITIALIZE 2
#define NOT_READY 3
#define PROBE_ERROR 10
#define CMD_NOT_OK 1
#define DATA_NOT_OK 2

//  ************      Owner  statuses        ************
#define FINISH 1   // Terminate session
#define CONTINUE 2 // Continue session
#define EXIT 3     // Exit While

//  ************      LEDS card  select        ************
#define LED_OFF ON // LED Off

#define LED_OFF_STATUS 0x00   // LED Off status
#define LED_ON_STATUS 0x01    // LED On status
#define LED_BLINK_STATUS 0x02 // LED Blink status

// #define  BAUDRATE           115200L

//  ************      General        ************
#define UART_0 1
#define UART_1 2

#define READ_ONLY const

// Macros
#define MIN(n, m) (((n) < (m)) ? (n) : (m))
#define MAX(n, m) (((n) < (m)) ? (m) : (n))
#define POWER(n) ((n) * (n))
#define DISTANCE(X, Y, Z) (POWER(X) + POWER(Y) + POWER(Z))

//  ************      SPI        ************
#define SPI_2_BUFF_LEN 50

//  ************      EEPROM  statuses        ************
#define EEREAD_OK 5    // E2 read params OK
#define EEREAD_ERROR 7 // E2 read params error (checksum)

//  ************      VERSIONS        ************
#define SW_HW_version 40 // Software and hardware version

//  ************      UART        ************
#define INVALID_COMMAND 0xFF
#define RENISHAW_CMD_MAX_LENGTH 200

//************** freeRTOS *****************
#define EVENT_DATA_READY 0x01 // 0x01: Packet received successfully
#define EVENT_ERROR 0x02      // 0x02: Timeout or Buffer Overflow

//  ************        H A R D W A R E      T E S T S       P I N S            ************

#endif // _CONSTANT_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
