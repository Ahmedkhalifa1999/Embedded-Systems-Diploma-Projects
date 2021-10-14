 /******************************************************************************
 *
 * Module: Ultrasonic Sensor
 *
 * File Name: ultrasonic.h
 *
 * Description: Header file for the ultrasonic driver
 *
 * Author: Ahmed Abd El-Moneam
 *
 *******************************************************************************/

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "std_types.h"

/*
 * Function to initialize ultrasonic sensor using ICU
 */
void Ultrasonic_init(void);

/*
 * Function to get ultrasonic sensor distance reading
 * Return: 16-bit distance value in cm
 */
uint16 Ultrasonic_readDistance(void);

#endif /* ULTRASONIC_H_ */
