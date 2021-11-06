/*
 * buzzer.h
 *
 * Buzzer Driver Header File
 *
 *  Created on: Nov 6, 2021
 *      Author: Ahmed abd El-Moneam Khalifa
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

#define Buzzer_PORT PORTD_ID
#define Buzzer_PIN PIN2_ID

void Buzzer_init();
void Buzzer_ON();
void Buzzer_OFF();

#endif /* BUZZER_H_ */
