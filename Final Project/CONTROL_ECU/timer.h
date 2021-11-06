/*
 * timer.h
 *
 * AVR Timer0 Driver Header File
 *
 *  Created on: Nov 6, 2021
 *      Author: Ahmed Abd El-Moneam Khalifa
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"

typedef enum {
	OVERFLOW = 0,
	COMPARE = 1
}Timer0_mode;

typedef enum {
	PRESCALAR_NO = 1,
	PRESCALAR_8,
	PRESCALAR_64,
	PRESCALAR_256,
	PRESCALAR_1024
}Timer0_prescalar;

typedef struct {
	Timer0_mode mode;
	int initialValue;
	int compareValue;
	Timer0_prescalar prescalar;
}Timer0_config;

void Timer0_init(Timer0_config *config);
void Timer0_setCallback(void (*user_callback)(void));
void Timer0_deinit();

#endif /* TIMER_H_ */
