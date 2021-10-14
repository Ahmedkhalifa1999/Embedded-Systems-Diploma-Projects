/*
 * main.c
 *
 *  Created on: Oct 14, 2021
 *      Author: ahmed
 */

#include "ultrasonic.h"
#include "lcd.h"
#include <avr/io.h>

int main (void) {
	uint16 distance = 0;

	/* Global Interrupt Enable */
	SREG |= (1<<7);


	/* Ultrasonic and LCD initialization */
	Ultrasonic_init();
	LCD_init();

	LCD_displayString("Distance =    cm");

	while(1) {
		distance = Ultrasonic_readDistance();
		LCD_moveCursor(0, 11);
		if (distance < 10) LCD_displayCharacter(' ');
		if (distance < 100) LCD_displayCharacter(' ');
		LCD_intgerToString(distance);
	}
}

