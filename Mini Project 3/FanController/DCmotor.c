 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: DCmotor.c
 *
 * Description: Source file for the DC motor driver
 *
 * Author: Ahmed Abd El-Moneam
 *
 *******************************************************************************/

#include "gpio.h"
#include "DCmotor.h"
#include "pwm.h"

/* Function to initialize DCmotor */
void DCmotor_init(void) {
	/* Set motor pins to output pins */
	GPIO_setupPinDirection(DCmotor_Port, DCmotor_PinA, PIN_OUTPUT);
	GPIO_setupPinDirection(DCmotor_Port, DCmotor_PinB, PIN_OUTPUT);
	/* Stop motor initially */
	GPIO_writePin(DCmotor_Port, DCmotor_PinA, 0);
	GPIO_writePin(DCmotor_Port, DCmotor_PinB, 0);
}

/* Function to rotate DCmotor
 * Arguments:
 * [In] state : direction of motor rotation (Clockwise, Anti-clockwise, Stop)
 * [In] speed : speed of motor rotation
 */
void DCmotor_rotate(DCmotor_state state, uint8 speed) {
	switch(state) {
	case STOP:
		GPIO_writePin(DCmotor_Port, DCmotor_PinA, 0);
		GPIO_writePin(DCmotor_Port, DCmotor_PinB, 0);
		break;
	case CW:
		GPIO_writePin(DCmotor_Port, DCmotor_PinA, 1);
		GPIO_writePin(DCmotor_Port, DCmotor_PinB, 0);
		break;
	case ACW:
		GPIO_writePin(DCmotor_Port, DCmotor_PinA, 0);
		GPIO_writePin(DCmotor_Port, DCmotor_PinB, 1);
	}
	/* Set Timer0 to produce desired PWM */
	PWM_Timer0_Start(speed);
}
