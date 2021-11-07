 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek & Ahmed Abd El-Moneam Khalifa
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/* Data type defining different available parity types */
typedef enum { /* number are shifted left by 4 to OR them directly when setting registers */
	NO_PARITY = 0 << 4,
	EVEN = 2 << 4,
	ODD = 3 << 4,
}UART_parity;

/* Data type defining different available Stop Bit counts*/
typedef enum { /* number are shifted left by 3 to OR them directly when setting registers */
	ONE = 0 << 3,
	TWO = 1 << 3
}UART_stopBits;

/* Data type defining UART configuration structure */
typedef struct {
	uint32 baudRate;
	UART_stopBits stopBits;
	UART_parity parity;
	uint8 dataSize; /* Number of data bits per frame */
}UART_config;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(UART_config* config);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_receiveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
