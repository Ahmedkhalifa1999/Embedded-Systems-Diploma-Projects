 /******************************************************************************
 *
 * Module: AVR Timer 0
 *
 * File Name: timer.c
 *
 * Description: Source file for AVR Timer 0 driver
 *
 * Author: Ahmed Abd El-Moneam Khalifa
 *
 *******************************************************************************/

#include "std_types.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

void (*callback)() = NULL_PTR; /* pointer to callback function */

/* function to initialize Timer 0 using configuration structure*/
void Timer0_init(Timer0_config *config) {
	TCCR0 |= (config -> mode) << 3; /* Set operation mode to Normal or CTC */
	OCR0 = config -> compareValue; /* Set compare value for Compare Mode */
	TCNT0 = config -> initialValue; /* Set initial timer value */
	TCCR0 |= config -> prescalar; /* Set prescalar */
	SET_BIT(TIMSK, config -> mode); /* Enable interrupt depending on mode */
}

/* function to set callback function for Timer 0 interrupt */
void Timer0_setCallback(void (*user_callback)()) {
	callback = user_callback;
}

/* function to deinitialize/stop Timer 0 */
void Timer0_deinit() {
	CLEAR_BIT(TIMSK, 0);
	CLEAR_BIT(TIMSK, 1);
	TCNT0 = 0;
	OCR0 = 0;
	TCCR0 = 0;
}

/* Interrupt Service Routines */
ISR(TIMER0_OVF_vect) {
	callback();
}

ISR(TIMER0_COMP_vect) {
	callback();
}
