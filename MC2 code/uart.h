 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	FIVE,SIX,SEVEN,EIGHT,NINE=7
}UART_BitN0;

typedef enum
{
	NO_PARITY,RESERVED,EVEN_PARITY,ODD_PARITY
}UART_Parity;

typedef enum
{
	ONE_BIT,TWO_BIT
}UART_StopBit;

typedef struct
{
	UART_BitN0 bit_number;
	UART_Parity parity;
	UART_StopBit stop_bit;
	uint32 baud_rate ;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const UART_ConfigType * Config_Ptr);

void UART_sendByte(const uint8 data);

uint8 UART_receiveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

void UART_SetCallBack(void(*a_ptr)(void));

#endif /* UART_H_ */
