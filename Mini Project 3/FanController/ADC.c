 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: ADC.c
 *
 * Description: Source file for the ADC driver
 *
 * Author: Ahmed Abd El-Moneam
 *
 *******************************************************************************/

#include "ADC.h"
#include <avr/io.h>

/*
 * Function used to initialize LCD according to Configuration in passed structure
 * Arguments:
 * [In] Config_Ptr : Pointer to structure containing configuration parameters with type defined in ADC.h
 */
void ADC_init(ADC_ConfigType* Config_Ptr) {
	/* Setting reference voltage according to option in passed structure */
	ADMUX = (ADMUX & 0x3F) | (ADMUX | (Config_Ptr -> ref_volt << 6));
	/* Enabling ADC */
	ADCSRA = (1<<ADEN);
	/* Setting frequency prescalar according to option in passed structure */
	ADCSRA = (ADCSRA & 0xF8) | Config_Ptr -> prescalar;
}

/*
 * Function to read from ADC
 * Arguments:
 * [In] ADCnumber : Number of ADC channel to read from (0 to 7)
 * Return: Returns 16-bit integer containing the 10-bit value read from the ADC
 */
uint16 ADC_readChannel(uint8 ADCnumber) {
	/* Return 0 if invalid ADD number is chosen */
	if (ADCnumber > 8) return 0;
	/* Setting ADC to use chosen channel */
	ADMUX |= ADCnumber;
	/* Setting ADC start conversion bit */
	ADCSRA |= (1<<ADSC);
	/* Waiting for ADC to finish Conversion */
	while (ADCSRA & (1 << ADSC));
	/* Return ADC result */
	return ADC;
}
