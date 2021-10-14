/*
 * main.c
 *
 *  Created on: Oct 10, 2021
 *      Author: ahmed
 */

#include "std_types.h"
#include "ADC.h"
#include "DCmotor.h"
#include "lm35_sensor.h"
#include "lcd.h"

int main(void) {
	/* Configuring ADC to use internal 2.56V reference and F_CPU/8 */
	ADC_ConfigType ADC_config = {INTERNAL256, PRESCALAR_8};
	ADC_init(&ADC_config);

	/* LCD initilization */
	LCD_init();

	/*DC motor initialization */
	DCmotor_init();

	uint8 temperature;

	/* Writing static text to LCD */
	LCD_displayString("Fan is ");
	LCD_moveCursor (1, 0);
	LCD_displayString("Temp = ");

	while(1) {
		temperature = LM35_getTemperature();
		LCD_moveCursor(1, 7);
		LCD_intgerToString(temperature);
		if (temperature < 30) {
			DCmotor_rotate(STOP, 0);
			LCD_displayStringRowColumn(0, 7, "OFF");
		}
		else if (temperature < 60) {
			DCmotor_rotate(CW, 25);
			LCD_displayStringRowColumn(0, 7, "ON ");
		}
		else if (temperature < 90) {
			DCmotor_rotate(CW, 50);
			LCD_displayStringRowColumn(0, 7, "ON ");
		}
		else if (temperature < 120) {
			DCmotor_rotate(CW, 75);
			LCD_displayStringRowColumn(0, 7, "ON ");
		}
		else {
			DCmotor_rotate(CW, 100);
			LCD_displayStringRowColumn(0, 7, "ON ");
		}
	}

	return 0;
}
