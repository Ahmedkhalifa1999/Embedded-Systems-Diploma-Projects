/*
 * CONTROL_main.c
 *
 *  Created on: Nov 6, 2021
 *      Author: Ahmed Abd El-Moneam Khalifa
 */

#include <avr/io.h>
#include "util/delay.h"
#include "twi.h"
#include "external_eeprom.h"
#include "timer.h"
#include "buzzer.h"
#include "DCmotor.h"
#include "uart.h"

uint16 timeCounter;
Timer0_config timer = {OVERFLOW, 0, 0, PRESCALAR_1024};

void timeIncrement();
void firstStartup();
boolean checkPassword(uint8* pass);
void savePassword(uint8* pass);
void openDoor();
void changePassword();
void soundAlarm();
void Buzz();

int main(void) {
	boolean passExists;
	char command;
	/* Hardware Initialization */
	SREG |= (1<<7);
	UART_config uart = {9600, ONE, NO_PARITY, 8};
	UART_init(&uart);
	TWI_config twi = {FAST, 0x01};
	TWI_init(&twi);
	DCmotor_init();
	Buzzer_init();

	/* Software Initialization */
	_delay_ms(500);
	 EEPROM_readByte(16, &passExists);
	if (passExists == 0x01) {
		UART_sendByte(0x00);
	}
	else {
		firstStartup();
	}

	while(1) {
		command = UART_receiveByte(); //Wait for command from HMI ECU
		switch (command) {
		case 0x05: /* Check Password */
			openDoor();
			break;
		case 0x08: /* Change Password */
			changePassword();
			break;
		case 0xFF:
			soundAlarm();
		}
	}


	return 0;
}

void timeIncrement() {
	timeCounter++;
}

void firstStartup() {
	uint8 pass[16];
	UART_sendByte(0x01);
	UART_receiveString(pass);
	savePassword(pass);
	EEPROM_writeByte(16, 0x01);
}

void savePassword(uint8* pass) {
	for (int i = 0; i < 16; i++) {
		EEPROM_writeByte(i, pass[i]);
		_delay_ms(10);
		if(pass[i] == 0) break;
	}
}

boolean checkPassword(uint8* pass) {
	uint8 current;
	for (int i = 0; i < 16; i++) {
		EEPROM_readByte(i, &current);
		_delay_ms(10);
		if (current != pass[i]) return FALSE;
		if (current == 0) break;
	}
	return TRUE;
}

void openDoor() {
	uint8 pass[16];
	UART_receiveString(pass);
	switch(checkPassword(pass)) {
	case TRUE:
		GPIO_writePin(PORTA_ID, PIN0_ID, LOGIC_HIGH);
		UART_sendByte(0x01);
		Timer0_init(&timer);
		Timer0_setCallback(timeIncrement);
		DCmotor_rotate(CW);
		timeCounter = 0;
		while(timeCounter < 58); /* Wait for motor to open door */
		UART_sendByte(0x0C);
		DCmotor_rotate(STOP);
		timeCounter = 0;
		while(timeCounter < 12); /* Leave door open for 3 seconds */
		UART_sendByte(0x0C);
		DCmotor_rotate(ACW);
		timeCounter = 0;
		while(timeCounter < 58); /* Wait for motor to close door */
		UART_sendByte(0x0C);
		DCmotor_rotate(STOP);
		Timer0_deinit();
		break;
	case FALSE:
		GPIO_writePin(PORTA_ID, PIN1_ID, LOGIC_HIGH);
		UART_sendByte(0x00);
	}
}

void changePassword() {
	uint8 oldPass[16];
	uint8 newPass[16];
	UART_receiveString(oldPass);
	switch (checkPassword(oldPass)) {
	case TRUE:
		UART_sendByte(TRUE);
		UART_receiveString(newPass);
		savePassword(newPass);
		break;
	case FALSE:
		UART_sendByte(FALSE);
	}
}

void soundAlarm() {
	timeCounter = 0;
	Timer0_init(&timer);
	Timer0_setCallback(Buzz);
}

void Buzz() {
	if (timeCounter & 0x01) Buzzer_ON();
	else Buzzer_OFF();
	timeCounter++;
	if (timeCounter == 229) {
		Buzzer_OFF();
		Timer0_deinit();
		UART_sendByte(0x00);
	}
}
