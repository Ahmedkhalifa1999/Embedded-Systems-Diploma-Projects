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

#include "buzzer.h"

/* Function to initialize Buzzer according to connected pin */
void Buzzer_init() {
	GPIO_setupPinDirection(Buzzer_PORT, Buzzer_PIN, PIN_OUTPUT);
	GPIO_writePin(Buzzer_PORT, Buzzer_PIN, LOGIC_LOW);
}

/* Function to turn the buzzer on */
void Buzzer_ON() {
	GPIO_writePin(Buzzer_PORT, Buzzer_PIN, LOGIC_HIGH);
}

/* Function to turn the buzzer off */
void Buzzer_OFF() {
	GPIO_writePin(Buzzer_PORT, Buzzer_PIN, LOGIC_LOW);
}
