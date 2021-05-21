 /******************************************************************************
 *
 * Module: TIMER0
 *
 * File Name: timer0.c
 *
 * Description: Source file for the TIMER0 driver
 *
 *******************************************************************************/

#include "timer0.h"

static  void (*g_normalCallBackPtr)(void) = NULL_PTR;
static  void (*g_compareCallBackPtr)(void) = NULL_PTR;
static uint8 g_clock ;

void Timer0_init(const Timer0_ConfigType * Config_Ptr)
{

	/*
	 * insert the required mode in the  TCCR0 Register
	 */
	TCCR0 = (TCCR0 & 0xB7) | (Config_Ptr->mode);

	/*
	 * insert the required clock value in global variable g_clock
	 */

	g_clock= Config_Ptr->clock ;
	/*
	 * insert the required initial value in the TCNT0 Register
	*/
	TCNT0 = Config_Ptr-> intial_value;
	/*
	 * insert the required compare value in the OCR0 Register
	*/
	OCR0 = Config_Ptr-> compare_value;
	TIMSK|= 3;
}


void Timer0_stop(void)
{
	/*
	* reset in the first three bits (CS10, CS11 and CS12) of TCCR0 Register
	*/
	TCCR0  &= 0xF8 ;
}
void Timer0_start(void)
{
	/*
	* reset the counter in register TCNT0
	*/
	TCNT0 = 0;
	/*
	* insert the required clock value in the first three bits (CS10, CS11 and CS12)
	* of TCCR0 Register
	*/
	TCCR0 = (TCCR0 & 0xF8) | g_clock ;
}

void Timer0_normalSetCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_normalCallBackPtr = a_ptr;
}
void Timer0_compareSetCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_compareCallBackPtr = a_ptr;
}


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_COMP_vect)
{
	if(g_compareCallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function */
		(*g_compareCallBackPtr)();
	}
}
ISR(TIMER0_OVF_vect)
{
	if(g_normalCallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function */
		(*g_normalCallBackPtr)();
	}
}

