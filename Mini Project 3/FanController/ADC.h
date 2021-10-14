 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: ADC.h
 *
 * Description: Header file for the ADC driver
 *
 * Author: Ahmed Abd El-Moneam
 *
 *******************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"

#define ADC_REF_VOLT_VALUE 2.56
#define ADC_MAXIMUM_VALUE 1023

typedef enum {
	AREF = 0, /*External Reference Voltage from AREF pin*/
	AVCC = 1, /*5V ADC Reference Voltage*/
	INTERNAL256 = 3 /*Internal 2.56 Reference Voltage*/

}ADC_ReferenceVoltage;

typedef enum {
	PRESCALAR_2 = 1,
	PRESCALAR_4,
	PRESCALAR_8,
	PRESCALAR_16,
	PRESCALAR_32,
	PRESCALAR_64,
	PRESCALAR_128
}ADC_Prescalar;


/*
 * ADC Configuration Structure to be used with ADC_init
 */
typedef struct {
	ADC_ReferenceVoltage ref_volt;
	ADC_Prescalar prescalar;
}ADC_ConfigType;

/*
 * Function used to initialize LCD according to Configuration in passed structure
 * Arguments:
 * [In] Config_Ptr : Pointer to structure containing configuration parameters with type defined above
 */
void ADC_init(ADC_ConfigType* Config_Ptr);

/*
 * Function to read from ADC
 * Arguments:
 * [In] ADCnumber : Number of ADC channel to read from (0 to 7)
 * Return: Returns 16-bit integer containing the 10-bit value read from the ADC
 */
uint16 ADC_readChannel(uint8 ADCnumber);

#endif /* ADC_H_ */
