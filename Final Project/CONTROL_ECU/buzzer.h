/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the Buzzer driver
 *
 * Author: Ahmed Abd El-Moneam Khalifa
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

/* Port and pin to which the buzzer is connected */
#define Buzzer_PORT PORTD_ID
#define Buzzer_PIN PIN2_ID

/* Function to initialize Buzzer according to connected pin */
void Buzzer_init();

/* Function to turn the buzzer on */
void Buzzer_ON();

/* Function to turn the buzzer off */
void Buzzer_OFF();

#endif /* BUZZER_H_ */
