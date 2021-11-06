/*
 * buzzer.c
 *
 * Buzzer Driver Source File
 *
 *  Created on: Nov 6, 2021
 *      Author: Ahmed abd El-Moneam Khalifa
 */

#include "buzzer.h"

void Buzzer_init() {
	GPIO_setupPinDirection(Buzzer_PORT, Buzzer_PIN, PIN_OUTPUT);
	GPIO_writePin(Buzzer_PORT, Buzzer_PIN, LOGIC_LOW);
}
void Buzzer_ON() {
	GPIO_writePin(Buzzer_PORT, Buzzer_PIN, LOGIC_HIGH);
}
void Buzzer_OFF() {
	GPIO_writePin(Buzzer_PORT, Buzzer_PIN, LOGIC_LOW);
}
