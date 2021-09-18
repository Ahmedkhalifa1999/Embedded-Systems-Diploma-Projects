#include <avr/io.h>
#include <avr/interrupt.h>
#include "time.h"
#include "sevseg.h"

time t = {0, 0, 0};

void Init();

int main(void) {
	Init();
	while(1) {
		refresh(&t);
	}
}

void Init() {
	DDRC |= 0x0F; //Set PORTC(3:0) to output pins to drive 7447 Decoder
	DDRA |= 0x3F; //Set PORTA(5:0) to output pins to drive Seven-Segment Selection
	DDRB &= ~(1 << PB2); //Set PIN 2 of PORT B to input for external interrupt
	DDRD &= ~((1 << PD2) | (1 << PD3)); //Set PINs 2 and 3 of PORT D to input for external interrupts
	PORTD |= (1 << PD2) | (1 << PD3); //Activate Internal Pull-Up Resistor for PINs 2 and 3 of PORT D
	MCUCR |= 0x0E; //Configure Interrupts 0 and 1 for appropriate events (Interrupt 2 had the appropriate
				   //configuration by default)
	GICR |= (1 << INT1) | (1 << INT0) | (1 << INT2); //Enable all external interrupts
	SREG |= (1 << 7); //Enable Interrupts (Global Interrupt Enable Bit)
	OCR1A = 15625; //set Compare A Match to appropriate value for a 64 pre-scalar (1Hz interrupts)
	TIMSK |= (1 << OCIE1A); //Enable Timer1 CompareA interrupt
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10); //Set clock pre-scalar to 64 and configure timer1 for
														//CTC mode
}

//Interrupt Service Routines
ISR(TIMER1_COMPA_vect) {
	incrementTime(&t);
}

ISR(INT0_vect) { //resets time
	t.seconds = 0;
	t.minutes = 0;
	t.hours = 0;
	TCNT1 = 0;
}

ISR(INT1_vect) { //Turns timer1 off (sets clock source to nothing) to pause
	TCCR1B &= ~(0x07);
}

ISR(INT2_vect) { //Turns timer1 on (sets clock source to pre-scalar 64) to resume
	TCCR1B |=  (1 << CS11) | (1 << CS10);
}
