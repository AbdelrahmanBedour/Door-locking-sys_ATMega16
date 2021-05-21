 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the BUZZER driver
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* BUZZER Port Configurations */
#define BUZZER_PORT_OUT PORTD
#define BUZZER_PORT_DIR DDRD
#define BUZZER_PIN PD5

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description: Function to initialize the BUZZER driver.
 */
void BUZZER_init(void);

/*
 * Description: Function to start the BUZZER .
 */
void BUZZER_start(void);

/*
 * Description: Function to stop the BUZZER .
 */
void BUZZER_stop(void);

#endif /* BUZZER_H_ */
