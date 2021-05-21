 /******************************************************************************
 *
 * Module: Micro-controller 1
 *
 * File Name: Micro1.c
 *
 * Description: Source file for Micro-controller 1
 *
 *******************************************************************************/
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "lcd.h"
#include "keypad.h"
#include "timer0.h"
#include "uart.h"

uint32 g_tick=0;

void Timer0_ISR(void)
{
	/* Increment the tick value */
	g_tick ++;
}


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
	uint8 password[6], pass_confirm[6], temp[10],Ack=0,key=0,state=0,i;

	/* Enable interrupts */
	SREG |= (1<<7);

	/* Create configuration structure for Timer0 driver
	 * pre-scaler=1024
	 * Compare mode
	 * Initial value = 0 ;
	 * Compare match =44    need 16 tick for 500 mili-second ;
	 */
	Timer0_ConfigType Timer0_Config = {F_CPU_1024,COMPARE,0,244};

	/* Create configuration structure for UART driver
	 * 8-Bits data
	 * No parity
	 * One stop bit
	 * Baud rate =9600
	 */
	UART_ConfigType UART_Config = {EIGHT,NO_PARITY,ONE_BIT,9600};

	/* Initialize LCD */
	LCD_init();

	/* Initialize Timer0 */
	Timer0_init(&Timer0_Config);

	/* Set Timer call back */
	Timer0_compareSetCallBack(Timer0_ISR);

	/* Initialize UART */
	UART_init(&UART_Config);

	while(1)
	{
		/* State 0
		 * User register with new password
		 */
		while (state==0)
		{
			/* Clear the screen */
			LCD_clearScreen();

			/* Ask the user to initialize password */
			LCD_displayString("Enter a new");
			LCD_displayStringRowColumn(1,0,"password");

			/* Get password */
			password[0]= KeyPad_getPressedKey();
			/* Clear the lcd */
			LCD_clearScreen();
			/* Display * on the screen as the user has pressed a button */
			LCD_displayCharacter('*');
			/* Half second delay for the keypad */
			Timer0_delayHalfSec(1);

			password[1]= KeyPad_getPressedKey();
			/* Display * on the screen as the user has pressed a button */
			LCD_displayCharacter('*');
			/* Half second delay for the keypad */
			Timer0_delayHalfSec(1);

			password[2]= KeyPad_getPressedKey();
			/* Display * on the screen as the user has pressed a button */
			LCD_displayCharacter('*');
			/* Half second delay for the keypad */
			Timer0_delayHalfSec(1);

			password[3]= KeyPad_getPressedKey();
			/* Display * on the screen as the user has pressed a button */
			LCD_displayCharacter('*');
			/* Half second delay for the keypad */
			Timer0_delayHalfSec(1);

			password[4]= KeyPad_getPressedKey();
			/* Display * on the screen as the user has pressed a button */
			LCD_displayCharacter('*');
			/* Half second delay for the keypad */
			Timer0_delayHalfSec(1);

			/* Add hash to indicate the end of the password */
			password[5]='#';

			/* Make sure that Micro 2 is ready */
			Ack = UART_receiveByte();

			/* Check the sent signal */
			if(Ack=='%')
			{
				/* Send password to Micro 2 */
				UART_sendString(password);

				/* reset the Ack */
				Ack=0;
			}

			/* Ask the user to confirm password after clearing the lcd */
			LCD_clearScreen();
			LCD_displayString("Re-enter");
			LCD_displayStringRowColumn(1,0,"password");

			/* Get password again to confirm it */
			pass_confirm[0]= KeyPad_getPressedKey();
			/* Clear the screen for the first time*/
			LCD_clearScreen();
			/* Display * on the screen as the user has pressed a button */
			LCD_displayCharacter('*');
			/* Half second delay for the keypad */
			Timer0_delayHalfSec(1);

			pass_confirm[1]= KeyPad_getPressedKey();
			/* Display * on the screen as the user has pressed a button */
			LCD_displayCharacter('*');
			/* Half second delay for the keypad */
			Timer0_delayHalfSec(1);

			pass_confirm[2]= KeyPad_getPressedKey();
			/* Display * on the screen as the user has pressed a button */
			LCD_displayCharacter('*');
			/* Half second delay for the keypad */
			Timer0_delayHalfSec(1);

			pass_confirm[3]= KeyPad_getPressedKey();
			/* Display * on the screen as the user has pressed a button */
			LCD_displayCharacter('*');
			/* Half second delay for the keypad */
			Timer0_delayHalfSec(1);

			pass_confirm[4]= KeyPad_getPressedKey();
			/* Display * on the screen as the user has pressed a button */
			LCD_displayCharacter('*');
			/* Half second delay for the keypad */
			Timer0_delayHalfSec(1);

			/* Add hash to indicate the end of the password */
			pass_confirm[5]='#';

			/* Make sure that Micro 2 is ready */
			Ack = UART_receiveByte();

			/* Check the sent signal */
			if(Ack=='%')
			{
				/* Send password to Micro 2 */
				UART_sendString(pass_confirm);

				/* reset the Ack */
				Ack=0;
			}

			/* Clear the screen */
			LCD_clearScreen();

			/* Get the state from Micro 2 */
			state = UART_receiveByte() ;

			/* Display "Success" if password match and "Try again" if password not match */
			if(state)
				 LCD_displayString("Success");
			else
				 LCD_displayString("Try again");

			/* Delay two second to show the result */
			Timer0_delayHalfSec(4);

			/* Reset the password and pass_confirm values */
			for (i=0;i<5;i++)
			{
				password[i]=0;
				pass_confirm[i]=0;
			}
		}

		/* State 1
		 * User choose between Change password (+) or open the door (-)
		 */
		while(state==1)
		{
			/* Clear the screen initially */
			LCD_clearScreen();

			/* Display state options */
			LCD_displayString("+ Change Pass");
			LCD_displayStringRowColumn(1,0,"- Open door");

			/* Get user's choice */
			key = KeyPad_getPressedKey();
			/* Half second delay for the keypad */
			Timer0_delayHalfSec(1);

			/* Check the pressed key if it (+) or (-) */
			if(key == '+')
			{
				/* Clear the screen initially */
				LCD_clearScreen();

				/* Display state options */
				LCD_displayString("Enter old pass");
				LCD_goToRowColumn(1,0);

				/* Get password */
				password[0]= KeyPad_getPressedKey();
				/* Display * on the screen as the user has pressed a button */
				LCD_displayCharacter('*');
				/* Half second delay for the keypad */
				Timer0_delayHalfSec(1);

				password[1]= KeyPad_getPressedKey();
				/* Display * on the screen as the user has pressed a button */
				LCD_displayCharacter('*');
				/* Half second delay for the keypad */
				Timer0_delayHalfSec(1);

				password[2]= KeyPad_getPressedKey();
				/* Display * on the screen as the user has pressed a button */
				LCD_displayCharacter('*');
				/* Half second delay for the keypad */
				Timer0_delayHalfSec(1);

				password[3]= KeyPad_getPressedKey();
				/* Display * on the screen as the user has pressed a button */
				LCD_displayCharacter('*');
				/* Half second delay for the keypad */
				Timer0_delayHalfSec(1);

				password[4]= KeyPad_getPressedKey();
				/* Display * on the screen as the user has pressed a button */
				LCD_displayCharacter('*');
				/* Half second delay for the keypad */
				Timer0_delayHalfSec(1);

				/* Add hash to indicate the end of the password */
				password[5]='#';

				/* Make sure that Micro 2 is ready */
				Ack = UART_receiveByte();

				/* Check the sent signal */
				if(Ack=='%')
				{
					/* Send option to Micro 2 */
					UART_sendByte('+');

					/* Send password to Micro 2 */
					UART_sendString(password);

					/* reset the Ack */
					Ack=0;
				}

				/* Get the state */
				state=UART_receiveByte();

				/* reset key */
				key=0;

				break;
			}

			/* Check the pressed key if it (+) or (-) */
			else if (key == '-')
			{
				/* Clearing the lcd */
				LCD_clearScreen();

				/* Display "Door is unlocking" */
				LCD_displayString("Door is");
				LCD_displayStringRowColumn(1,0,"unlocking");

				/* Make sure that Micro 2 is ready */
				Ack = UART_receiveByte();

				/* Check the sent signal */
				if(Ack=='%')
				{
					/* Send option to Micro 2 */
					UART_sendByte('-');

					/* reset the Ack */
					Ack=0;
				}

				/* delay 15 seconds while the door is unlocking */
				Timer0_delayHalfSec(30);

				/* Clearing the lcd */
				LCD_clearScreen();

				/* delay 3 seconds waiting for the User to enter */
				Timer0_delayHalfSec(6);

				/* Display Door is locking */
				LCD_displayString("Door is");
				LCD_displayStringRowColumn(1,0,"locking");

				/* delay 15 seconds to lock the door back */
				Timer0_delayHalfSec(30);

				/* reset key */
				key=0;

				break;
			}

			/* Reset the password and pass_confirm values */
			for (i=0;i<5;i++)
			{
				password[i]=0;
				pass_confirm[i]=0;
			}
		}

		/* State 2
		 * The User has entered the password wrong for 3 times
		 * System freezes for 1 minute
		 */
		while(state==2)
		{
			/* Clear the screen initially */
			LCD_clearScreen();

			/* Display "ERROR" message on the screen */
			LCD_displayString("ERROR");

			/* 1 minute delay to freeze the system */
			Timer0_delayHalfSec(120);

			/* System get back to work by setting the state back to 1 */
			state=1;
		}
	}

	return 0;
}

