/*
 * timer.c
 *
 * AVR Timer0 Driver Source File
 *
 *  Created on: Nov 6, 2021
 *      Author: Ahmed Abd El-Moneam Khalifa
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

void (*callback)();

void Timer0_init(Timer0_config *config) {
	OCR0 = config -> compareValue;
	TCNT0 = config -> initialValue;
	TCCR0 = config -> prescalar;
	SET_BIT(TIMSK, config -> mode);
}

void Timer0_setCallback(void (*user_callback)()) {
	callback = user_callback;
}

void Timer0_deinit() {
	CLEAR_BIT(TIMSK, 0);
	CLEAR_BIT(TIMSK, 1);
	TCNT0 = 0;
	OCR0 = 0;
	TCCR0 = 0;
}


ISR(TIMER0_OVF_vect) {
	callback();
}

ISR(TIMER0_COMP_vect) {
	callback();
}
