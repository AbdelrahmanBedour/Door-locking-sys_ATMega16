 /******************************************************************************
 *
 * Module: TiMER0
 *
 * File Name: timer0.h
 *
 * Description: Header file for the TIMER driver
 *
 *******************************************************************************/


#ifndef TIMER0_H_
#define TIMER0_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	NORMAL,COMPARE=8
}Timer0_Mode;
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer0_Clock;


typedef struct
{
	Timer0_Clock clock;
	Timer0_Mode mode;
	uint8 intial_value;
	uint8 compare_value ;
}Timer0_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the TIMER driver
 * 	1. Set the required clock.
 * 	2. Set the required mode.
 * 	3. Set timer initial value.
 * 	4. Set compare register.
 */
void Timer0_init(const Timer0_ConfigType * Config_Ptr);
/*
 * Description: Function to start the timer.
 */

void Timer0_start(void);

/*
 * Description: Function to stop the timer.
 */
void Timer0_stop(void);

/*
 * Description: Function to set the Call Back function address.
 */
void Timer0_normalSetCallBack(void(*a_ptr)(void));

/*
 * Description: Function to set the Call Back function address.
 */
void Timer0_compareSetCallBack(void(*a_ptr)(void));



#endif /* TIMER0_H_ */
