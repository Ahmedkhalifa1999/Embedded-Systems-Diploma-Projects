#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define REFRESH_DELAY _delay_us(10);

struct timeData {
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hours;
}time;

void incrementTime(struct timeData* time) {
	time -> seconds++;
	if (time -> seconds == 60) {
		time -> seconds = 0;
		time -> minutes++;
	}
	if (time -> minutes == 60) {
		time -> minutes = 0;
		time -> hours++;
	}
	if (time -> hours == 100) time -> hours = 0;
}

ISR(TIMER1_COMPA_vect) {
	incrementTime(&time);
}

ISR(INT0_vect) {
	time.seconds = 0;
	time.minutes = 0;
	time.hours = 0;
	TCNT1 = 0;
}

ISR(INT1_vect) {
	TCCR1B &= ~(0x07);
}

ISR(INT2_vect) {
	TCCR1B |=  (1 << CS11) | (1 << CS10);
}

int main(void) {
	OCR1A = 15625;
	TIMSK |= (1 << OCIE1A);
	DDRC |= 0x0F;
	DDRA |= 0x3F;
	DDRB &= ~(1 << PB2);
	DDRD &= ~((1 << PD2) | (1 << PD3));
	PORTD |= (1 << PD2) | (1 << PD3);
	MCUCR |= 0x0E;
	GICR |= (1 << INT1) | (1 << INT0) | (1 << INT2);
	SREG |= (1 << 7);
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);
	while(1) {
		PORTA = (PORTA & 0b11000000) | 1;
		PORTC = (PORTC & 0xF0) | (time.seconds % 10);
		REFRESH_DELAY
		PORTA = (PORTA & 0b11000000) | 2;
		PORTC = (PORTC & 0xF0) | (time.seconds / 10);
		REFRESH_DELAY
		PORTA = (PORTA & 0b11000000) | 4;
		PORTC = (PORTC & 0xF0) | (time.minutes % 10);
		REFRESH_DELAY
		PORTA = (PORTA & 0b11000000) | 8;
		PORTC = (PORTC & 0xF0) | (time.minutes / 10);
		REFRESH_DELAY
		PORTA = (PORTA & 0b11000000) | 16;
		PORTC = (PORTC & 0xF0) | (time.hours % 10);
		REFRESH_DELAY
		PORTA = (PORTA & 0b11000000) | 32;
		PORTC = (PORTC & 0xF0) | (time.hours / 10);
		REFRESH_DELAY
	}
}
