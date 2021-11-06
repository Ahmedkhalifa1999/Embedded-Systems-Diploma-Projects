/******************************************************************************
 *
 * Module: DC motor
 *
 * File Name: DCmotor.h
 *
 * Description: Header file for the DC motor driver
 *
 * Author: Ahmed Abd El-Moneam
 *
 *******************************************************************************/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#define DCmotor_Port PORTB_ID
#define DCmotor_PinA PIN0_ID
#define DCmotor_PinB PIN1_ID

typedef enum{
	CW, /* Clockwise */
	ACW, /* Anti-clockwise */
	STOP
}DCmotor_state;

/* Function to initialize DCmotor */
void DCmotor_init(void);

/* Function to rotate DCmotor
 * Arguments:
 * [In] state : direction of motor rotation (Clockwise, Anti-clockwise, Stop)
 * [In] speed : speed of motor rotation
 */
void DCmotor_rotate(DCmotor_state state);

#endif /* DCMOTOR_H_ */
