 /******************************************************************************
 *
 * Module: MOTOR
 *
 * File Name: motor.c
 *
 * Description: Source file for the  MOTOR driver
 *
 *******************************************************************************/

#include "motor.h"

void MOTOR_init(void)
{
	/* configure IN1 and IN2 as output pins */
	MOTOR_PORT_DIR |= (1<<MOTOR_IN1) | (1<<MOTOR_IN2);

	/* Motor is stopped at the beginning */
	MOTOR_PORT_OUT &= (~(1<<MOTOR_IN1) )& (~(1<<MOTOR_IN2)) ;
}



void MOTOR_rotateCW(void)
{
	/* reset IN1 and set IN2 to rotate clockwise */

	MOTOR_PORT_OUT &= (~(1<<MOTOR_IN1) );
	MOTOR_PORT_OUT |= (1<<MOTOR_IN2) ;


}



void MOTOR_rotateACW(void)
{
	/* set IN1 and reset IN2 to rotate Anti-clockwise */
	MOTOR_PORT_OUT |= (1<<MOTOR_IN1) ;
	MOTOR_PORT_OUT &= (~(1<<MOTOR_IN2) );


}

void MOTOR_stop(void)
{
	/* reset IN1 and IN2 to stop the Motor */
	MOTOR_PORT_OUT &= (~(1<<MOTOR_IN1) )& (~(1<<MOTOR_IN2)) ;

}


