 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the BUZZER driver
 *
 *******************************************************************************/

#include "buzzer.h"

void BUZZER_init(void)
{
	/* configure Buzzer PIN as output pin */
	BUZZER_PORT_DIR |= (1<<BUZZER_PIN) ;

	/*Buzzer is stopped at the beginning */
	BUZZER_PORT_OUT &= (~(1<<BUZZER_PIN)  ) ;
}


void BUZZER_start(void)
{
	/* set Buzzer Pin to  start  */
	BUZZER_PORT_OUT |= (1<<BUZZER_PIN)   ;
}

void BUZZER_stop(void)
{
	/* reset Buzzer Pin to stop  */
	BUZZER_PORT_OUT &= (~(1<<BUZZER_PIN)  ) ;
}
