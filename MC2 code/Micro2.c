 /******************************************************************************
 *
 * Module: Micro-controller 2
 *
 * File Name: Micro2.c
 *
 * Description: Source file for Micro-controller 2
 *
 *******************************************************************************/
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "timer0.h"
#include "uart.h"
#include "motor.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include "I2c.h"

uint32 g_tick=0;

void Timer0_ISR(void)
{
	/* Increment the tick value */
	g_tick ++;
}

/* Function to use the Timer as a delay by half second */
void Timer0_delayHalfSec(uint8 half_sec)
{
	/* Check if tick = 0 to start the timer */
	if (g_tick==0)
	{
		/* Start the timer */
		Timer0_start();
	}

	/* Wait until the timer counts to the desired value */
	while ( g_tick != (half_sec*16)  ) {}

	/* Stop the timer*/
	Timer0_stop();

	/* Reset the tick value */
	g_tick=0;
}

int main(void)
{
	uint8 password[6] ,pass_confirm[6],state=0,key=0,wrong_entry_count=0, i;

	/* Enable interrupts */
	SREG |= (1<<7);

	/* Create configuration structure for Timer0 driver
	 * pre-scaler=1024
	 * Compare mode
	 * Initial value = 0 ;
	 * Compare match =244   needs 16 tick for 500 mili-second ;
	 */
	Timer0_ConfigType Timer0_Config = {F_CPU_1024,COMPARE,0,244};

	/* Create configuration structure for UART driver
	 * 8-Bits data
	 * No parity
	 * One stop bit
	 * Baud rate =9600
	 */
	UART_ConfigType UART_Config = {EIGHT,NO_PARITY,ONE_BIT,9600};

	/* Create configuration structure for I2C driver
	 * 400 kbps rate
	 * Slave address = 0x01
	 */
	I2C_ConfigType I2C_Config = {400000,0x01};

	/* Initialize Buzzer */
	BUZZER_init();

	/* Initialize Motor */
	MOTOR_init();

	/* Initialize Timer0 */
	Timer0_init(&Timer0_Config);

	/* Set Timer call back */
	Timer0_compareSetCallBack(Timer0_ISR);

	/* Initialize UART */
	UART_init(&UART_Config);

	/* Initialize EEPROM */
	EEPROM_init(&I2C_Config);

	while (1)
	{
		/* State 0
		 * Micro 2 compares the password
		 */
		while(state==0)
		{
			/* Reset wrong entry count and begin again*/
			wrong_entry_count=0;

			/* delay 1 sec */
			Timer0_delayHalfSec(2);

			/* Send % as Micro 2 is ready */
			UART_sendByte('%');

			/* Waiting for the Password */
			UART_receiveString(password);

			/* Send % as Micro 2 is ready */
			UART_sendByte('%');

			/* Waiting for the Password confirmation */
			UART_receiveString(pass_confirm);

			/* Check the password match */
			state=1;
			for(i=0;i<5;i++)
				{
					if (password[i]!=pass_confirm[i])
					{
						state =0;
						break;
					}
				}

			/* Send the state to Micro 1 */
			UART_sendByte(state);

			/* Check the if the password matches to write it back in the EEPROM */
			if(state)
			{
				for (i=0;i<5;i++)
				{
					/* Write the password in the EEPROM */
					EEPROM_writeByte(i,password[i]);
				}
			}
		}

		/* State 1
		 * Micro 2 opens the door or changes the password
		 */
		while(state==1)
		{
			/* Send % as Micro 2 is ready */
			UART_sendByte('%');

			/* Waiting for the option */
			key = UART_receiveByte();

			/* Check the pressed key if it (+) or (-) */
			if(key=='+')
			{
				/* Waiting for the Password */
				UART_receiveString(password);

				/* Check the password*/
				state=0;
				for (i=0;i<5;i++)
				{
					/* Read the password from EEPROM */
					EEPROM_readByte(i,pass_confirm[i]);

					/* Compare the password */
					if (password[i]!=pass_confirm[i])
					{
						/* Stay in state 1 */
						state =1;

						/* Increment the wrong_entry_count variable */
						wrong_entry_count++;

						break;
					}
				}

				/* Check if the User has entered password wrong for 3 times */
				if(wrong_entry_count==3)
				{
					/* Go to state 2 to freeze the system and activating the Buzzer */
					state=2;

					/* Reset the wrong_entry_count variable */
					wrong_entry_count=0;
				}

				/* Send the state to Micro 1 */
				UART_sendByte(state);
			}

			/* Check the pressed key if it (+) or (-) */
			else if(key=='-')
			{
				/* Start Motor Clockwise */
				MOTOR_rotateCW();

				/* Delay 15 seconds */
				Timer0_delayHalfSec(30);

				/* Stop the Motor */
				MOTOR_stop();

				/* 3 seconds delay */
				Timer0_delayHalfSec(6);

				/* Start Motor Anti-Clockwise */
				MOTOR_rotateACW();

				/* Delay 15 seconds */
				Timer0_delayHalfSec(30);

				/* Stop the Motor */
				MOTOR_stop();
			}
		}

		/* State 2
		 * Micro 2 the Buzzer for 1 minute
		 */
		while(state==2)
		{
			/* Start the Buzzer */
			BUZZER_start();

			/* Delay 1 minute */
			Timer0_delayHalfSec(120);

			/* Stop the Buzzer */
			BUZZER_stop();

			/* Get back to state 1 */
			state=1;
		}
	}

	return 0;
}
