 /******************************************************************************
 *
 * Module: MOTOR
 *
 * File Name: motor.h
 *
 * Description: Header file for the MOTOR driver
 *
 *******************************************************************************/
#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"


/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/



/* MOTOR Port Configurations */
#define MOTOR_PORT_OUT PORTD
#define MOTOR_PORT_DIR DDRD
#define MOTOR_IN1 PD6
#define MOTOR_IN2 PD7

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description: Function to initialize the MOTOR driver.
 */
void MOTOR_init(void);

/*
 * Description: Function to rotate the MOTOR clockwise.
 */
void MOTOR_rotateCW(void);

/*
 * Description: Function to rotate the MOTOR Anti-clockwise.
 */
void MOTOR_rotateACW(void);

/*
 * Description: Function to Stop the MOTOR .
 */
void MOTOR_stop(void);

#endif /* MOTOR_H_ */
