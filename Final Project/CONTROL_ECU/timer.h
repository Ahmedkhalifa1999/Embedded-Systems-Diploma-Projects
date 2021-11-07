/******************************************************************************
 *
 * Module: AVR Timer 0
 *
 * File Name: timer.h
 *
 * Description: Header file for AVR Timer 0 driver
 *
 * Author: Ahmed Abd El-Moneam Khalifa
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"

/* Data type defining Timer 0 available operation modes*/
typedef enum {
	OVERFLOW = 0,
	COMPARE = 1
}Timer0_mode;

/* Data type defining Timer 0 available prescalars */
typedef enum {
	PRESCALAR_NO = 1,
	PRESCALAR_8,
	PRESCALAR_64,
	PRESCALAR_256,
	PRESCALAR_1024
}Timer0_prescalar;

/* Data type defining Timer 0 configuration structure */
typedef struct {
	Timer0_mode mode; /* Operation mode */
	int initialValue; /* Starting Value */
	int compareValue; /* Compare Value (Useful for Compare mode only) */
	Timer0_prescalar prescalar; /* prescalar */
}Timer0_config;

/* function to initialize Timer 0 using configuration structure*/
void Timer0_init(Timer0_config *config);

/* function to set callback function for Timer 0 interrupt */
void Timer0_setCallback(void (*user_callback)(void));

/* function to deinitialize/stop Timer 0 */
void Timer0_deinit();

#endif /* TIMER_H_ */
