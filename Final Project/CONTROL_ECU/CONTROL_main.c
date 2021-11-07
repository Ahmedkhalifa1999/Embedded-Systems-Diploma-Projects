 /******************************************************************************
 *
 * Module: Control ECU for Door Lock
 *
 * File Name: CONTROL_main.c
 *
 * Description: Application for Control ECU for Door Lock
 *
 * Author: Ahmed Abd El-Moneam Khalifa
 *
 *******************************************************************************/

#include <avr/io.h>
#include "std_types.h"
#include "util/delay.h"
#include "uart.h"
#include "twi.h"
#include "external_eeprom.h"
#include "timer.h"
#include "buzzer.h"
#include "DCmotor.h"

/* Timer 0 variables:
 * -timeCounter : counter for Callback to increment every 8ms
 * -timer : Timer0 configuration structure for initialization function calls
 *
 * Timer 0 is operated in compare mode with callback incrementing timeCounter every 8ms according
 * to compare value (250) and prescalar (256) with 8MHz CPU Frequency
 */
uint16 timeCounter;
Timer0_config timer = {COMPARE, 0, 250, PRESCALAR_256};

/* Timer 0 call back function to increment time counter for door opening procedure */
void timeIncrement();

/* Timer 0 call back function to operate buzzer */
void Buzz();

/* First Startup initialization function */
void firstStartup();

/* Checks passed password against password saved in EEPROM */
boolean checkPassword(uint8* pass);

/* Saves passed password in EEPROM */
void savePassword(uint8* pass);

/* Responds to Check Password command (Open Door) from HMI ECU */
void openDoor();

/* Responds to Change Password command from HMI ECU */
void changePassword();

/* Responds to Sound Alarm command from HMI ECU */
void soundAlarm();

int main(void) {
	boolean passExists;
	char command;
	/* Hardware Initialization */
	SREG |= (1<<7);
	UART_config uart = {9600, ONE, NO_PARITY, 8};
	UART_init(&uart);
	TWI_config twi = {NORMAL, 0x01}; /* 100kbps speed for EEPROM */
	TWI_init(&twi);
	DCmotor_init();
	Buzzer_init();

	/* Software Initialization */
	_delay_ms(1000); /* Wait for HMI ECU to boot up */
	 EEPROM_readByte(16, &passExists); /* Check for flag indicating that a password exists */
	if (passExists == 0x01) {
		/* Send to HMI ECU indicating that a password exists */
		UART_sendByte(0x00);
	}
	else {
		/* call first startup initialization function */
		firstStartup();
	}

	while(1) {
		command = UART_receiveByte(); //Wait for command from HMI ECU
		switch (command) {
		case 0x05: /* Check Password (Open Door) */
			openDoor();
			break;
		case 0x08: /* Change Password */
			changePassword();
			break;
		case 0xFF: /* Sound Alarm */
			soundAlarm();
		}
	}

	return 0;
}

/* Timer 0 call back function to increment time counter for door opening procedure */
void timeIncrement() {
	timeCounter++;
}

/* Timer 0 call back function to operate buzzer */
void Buzz() {
	/* Toggle Buzzer every 1 second (1024ms) */
	if (timeCounter & (1<<7)) Buzzer_ON();
	else Buzzer_OFF();

	/* increment time Counter to count time (8ms have passed) */
	timeCounter++;

	/* if counter reaches 7500 that means 60 seconds have passed */
	if (timeCounter == 7500) {
		Buzzer_OFF(); /* Turn off buzzer */
		Timer0_deinit(); /* Turn Off and de-initialize Timer 0*/

		/* Signal to HMI ECU that 60 seconds have passed for it to return to normal operation */
		UART_sendByte(0x00);
	}
}

/* First Startup initialization function */
void firstStartup() {
	uint8 pass[16];
	UART_sendByte(0x01); /* Send to HMI ECU to signal that no password exists (first startup) */
	UART_receiveString(pass); /* Wait for new password from HMI ECU */
	savePassword(pass); /* Save password to EEPROM */
	EEPROM_writeByte(16, 0x01); /* Set password exists flag in EEPROM */
}

/* Checks passed password against password saved in EEPROM */
boolean checkPassword(uint8* pass) {
	uint8 current; /* Variable to save current character read from EEPROM */
	for (int i = 0; i < 16; i++) {
		/* Read character from EEPROM and wait for 10ms to avoid overwhelming EEPROM with commands */
		EEPROM_readByte(i, &current);
		_delay_ms(10);

		/* Return false on mismatch between passed password and that saved in EEPROM */
		if (current != pass[i]) return FALSE;

		/* Break to return true on Null character indicating end of password */
		if (current == 0) break;
	}
	return TRUE;
}

/* Saves passed password in EEPROM */
void savePassword(uint8* pass) {
	for (int i = 0; i < 16; i++) {
		/* Write character to EEPROM and wait for 10ms to avoid overwhelming EEPROM with commands */
		EEPROM_writeByte(i, pass[i]);
		_delay_ms(10);

		/* Break on Null character indicating end of password */
		if(pass[i] == 0) break;
	}
}

/* Responds to Check Password command (Open Door) from HMI ECU */
void openDoor() {
	uint8 pass[16];
	UART_receiveString(pass); /* Receive entered password from HMI ECU */

	/* Check entered password */
	switch(checkPassword(pass)) {
	case TRUE:
		/* Door opening procedure if password is correct */

		/* Send to HMI ECU signaling correct password */
		UART_sendByte(0x01);

		/* Initialize Timer 0 for use in door opening procedure */
		Timer0_init(&timer);
		Timer0_setCallback(timeIncrement);

		/* Start opening door */
		DCmotor_rotate(CW); /* Rotate motor Clockwise */
		timeCounter = 0;
		while(timeCounter <= 1875); /* Wait for motor to open door (15 seconds) */
		UART_sendByte(0x0C); /* Signal to HMI ECU that door is open */

		/* Leave door open for 3 seconds */
		DCmotor_rotate(STOP); /* Stop motor */
		timeCounter = 0;
		while(timeCounter <= 375); /* Wait for 3 seconds */
		UART_sendByte(0x0C); /* Signal to HMI ECU that door will start closing */

		/* Start closing door */
		DCmotor_rotate(ACW); /* Rotate motor Anti-clockwise */
		timeCounter = 0;
		while(timeCounter <= 1875); /* Wait for motor to close door */
		UART_sendByte(0x0C); /* Signal to HMI ECU that door is closed */

		/* Stop motor and de-initialize Timer 0 to end procedure */
		DCmotor_rotate(STOP);
		Timer0_deinit();
		break;
	case FALSE:
		UART_sendByte(0x00); /* Send to HMI ECU signaling wrong password */
	}
}

/* Responds to Change Password command from HMI ECU */
void changePassword() {
	uint8 oldPass[16];
	uint8 newPass[16];

	/* Receive old password from HMI ECU and check if it's correct */
	UART_receiveString(oldPass);
	switch (checkPassword(oldPass)) {
	case TRUE: /* Password is correct */
		UART_sendByte(0x01); /* Send to HMI ECU signaling correct password */
		UART_receiveString(newPass); /* REceive new password from HMI ECU */
		savePassword(newPass); /* Save new password */
		break;
	case FALSE: /* Password is wrong */
		UART_sendByte(0x00); /* Send to HMI ECU signaling wrong password */
	}
}

/* Responds to Sound Alarm command from HMI ECU */
void soundAlarm() {
	/* Initialize timer counter and timer 0 module */
	timeCounter = 0;
	Timer0_init(&timer);

	/* Set Timer 0 callback as buzzer so it starts beeping */
	Timer0_setCallback(Buzz);
}
