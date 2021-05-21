 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 *******************************************************************************/

#include "uart.h"

static  void (*g_CallBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(USART_RXC_vect)
{
	if(g_CallBackPtr != NULL_PTR)
		{
			/* Call the Call Back function */
			(*g_CallBackPtr)();
		}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void UART_SetCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_CallBackPtr = a_ptr;
}

void UART_init(const UART_ConfigType * Config_Ptr)
{
	uint32 baud_prescale= (  F_CPU / (  (Config_Ptr->baud_rate) * 8) )   - 1 ;
	/************************** UCSRB Description **************************
	* U2X = 1 for double transmission speed
	************************************************************************/
	UCSRA = (1<<U2X);
	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART TX Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * Adjust UCSZ2 for the data mode
	 * RXB8 & TXB8 =0
	 ***********************************************************************/ 
	UCSRB =  (1<<RXEN) | (1<<TXEN);
	UCSRB = (UCSRB & (~(0x04)) ) | (Config_Ptr->bit_number & 0x04 );
	
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * Adjust UPM1:0 for parity bits
	 * Adjust USBS for stop bit
	 * Adjust UCSZ1:0 for data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/ 	
	UCSRC = (1<<URSEL) | (((Config_Ptr->parity) & 0x03)<<4 ) | (((Config_Ptr->stop_bit) &0x01 )<<3) | (((Config_Ptr->bit_number) & 0x03 )<<1);
	
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = baud_prescale>>8;
	UBRRL = baud_prescale;

}
	
void UART_sendByte(const uint8 data)
{
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for 
	 * transmitting a new byte so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	/* Put the required data in the UDR register and it also clear the UDRE flag as 
	 * the UDR register is not empty now */	 
	UDR = data;
}

uint8 UART_receiveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this 
	 * flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/* Read the received data from the Rx buffer (UDR) and the RXC flag 
	   will be cleared after read this data */	 
    return UDR;		
}

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = UART_receiveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_receiveByte();
	}
	Str[i] = '\0';
}

