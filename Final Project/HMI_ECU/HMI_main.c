/*
 * HMI_main.c
 *
 *  Created on: Nov 6, 2021
 *      Author: Ahmed Abd El-Moneam Khalifa
 */

#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "util/delay.h"

boolean checkPassword(uint8* pass);
void getPassword(uint8* pass);
void soundAlarm();
void firstStartup();
void openDoor();
void changePassword();

int main(void) {
	/* Hardware Initialization */
	LCD_init();
	UART_config uart = {9600, ONE, NO_PARITY, 8};
	UART_init(&uart);

	/* Software Initialization */
	if (UART_receiveByte() == 0x01) {
		/* if the Control MCU sends 0x01 on the UART, this means that there is no previous password saved,
		 * and thus HMI MCU will go into First Startup Sequence
		 */
		firstStartup();
	}

	while(1) {
		/* Normal Operation Sequence
		 * Waiting for user to choose an option
		 */
		LCD_clearScreen();
		LCD_displayString("+ : Open Door");
		LCD_moveCursor(1, 0);
		LCD_displayString("- : Change Pass");
		switch (KEYPAD_getPressedKey()) {
		case '+' :
			_delay_ms(500);
			openDoor();
			break;
		case '-' :
			_delay_ms(500);
			changePassword();
		}
	}

	return 0;
}

boolean checkPassword(uint8* pass) {
	UART_sendByte(0x05);
	UART_sendString(pass);
	return UART_receiveByte();
}

void getPassword(uint8* pass) {
	uint8 key;
	for (int i = 0; i < 16; i++) {
		key = KEYPAD_getPressedKey();
		if (key == 13) {
			pass[i] = 0;
			break;
		}
		LCD_displayCharacter('*');
		pass[i] = key + 48;
		_delay_ms(500);
	}
}

void soundAlarm() {
	UART_sendByte(0xFF);
	LCD_clearScreen();
	LCD_displayString("Intruder Alert!");
	while (UART_receiveByte() != 0x00);
}

void firstStartup() {
	uint8 pass[16];
	LCD_displayString("New Password:");
	LCD_moveCursor(1, 0);
	getPassword(pass);
	UART_sendString(pass);
}

void openDoor() {
	uint8 pass[16];
	static uint8 wrongCount = 0;
	LCD_clearScreen();
	LCD_displayString("Password:");
	LCD_moveCursor(1,0);
	getPassword(pass);
	if (checkPassword(pass) == 0x01) {
		LCD_clearScreen();
		LCD_displayString("Opening Door");
		while (UART_receiveByte() != 0x0C);
		LCD_clearScreen();
		LCD_displayString("Door Open");
		while (UART_receiveByte() != 0x0C);
		LCD_clearScreen();
		LCD_displayString("Closing Door");
		while (UART_receiveByte() != 0x0C);
		wrongCount = 0;
	}
	else {
		LCD_clearScreen();
		LCD_displayString("Wrong Password!");
		wrongCount++;
		if (wrongCount == 3) {
			soundAlarm();
			wrongCount = 0;
		}
		else {
			_delay_ms(1000);
			openDoor();
		}
	}
}

void changePassword() {
	uint8 oldPass[16];
	uint8 newPass[16];
	static uint8 wrongCount = 0;
	LCD_clearScreen();
	LCD_displayString("Old Password:");
	LCD_moveCursor(1,0);
	getPassword(oldPass);
	UART_sendByte(0x08);
	UART_sendString(oldPass);
	if (UART_receiveByte() == 0x01) {
		LCD_clearScreen();
		LCD_displayString("New Password:");
		LCD_moveCursor(1,0);
		wrongCount = 0;
		getPassword(newPass);
		UART_sendString(newPass);
	}
	else {
		LCD_clearScreen();
		LCD_displayString("Wrong Password!");
		wrongCount++;
		if (wrongCount == 3) {
			soundAlarm();
			wrongCount = 0;
		}
		else {
			_delay_ms(1000);
			changePassword();
		}
	}
}



