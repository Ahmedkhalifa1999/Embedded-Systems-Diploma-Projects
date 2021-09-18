#include <avr/io.h>
#include <util/delay.h>
#include "time.h"

#define REFRESH_DELAY _delay_us(10); //Macro for delays to avoid overlapping

void refresh(time* t) {
	PORTA = (PORTA & 0xC0) | 1;
	PORTC = (PORTC & 0xF0) | (t -> seconds % 10); //refresh lower digit of seconds
	REFRESH_DELAY
	PORTA = (PORTA & 0xC0) | 2;
	PORTC = (PORTC & 0xF0) | (t -> seconds / 10); //refresh higher digit of seconds
	REFRESH_DELAY
	PORTA = (PORTA & 0xC0) | 4;
	PORTC = (PORTC & 0xF0) | (t -> minutes % 10); //refresh lower digit of minutes
	REFRESH_DELAY
	PORTA = (PORTA & 0xC0) | 8;
	PORTC = (PORTC & 0xF0) | (t -> minutes / 10); //refresh higher digit of minutes
	REFRESH_DELAY
	PORTA = (PORTA & 0xC0) | 16;
	PORTC = (PORTC & 0xF0) | (t -> hours % 10); //refresh lower digit of hours
	REFRESH_DELAY
	PORTA = (PORTA & 0xC0) | 32;
	PORTC = (PORTC & 0xF0) | (t -> hours / 10); //refresh higher digit of hours
	REFRESH_DELAY
}
