 /******************************************************************************
 *
 * Module: HMI ECU for Door Lock
 *
 * File Name: HMI_main.c
 *
 * Description: Application for HMI ECU for Door Lock
 *
 * Author: Ahmed Abd El-Moneam Khalifa
 *
 *******************************************************************************/

#include "std_types.h"
#include "util/delay.h"
#include "keypad.h"
#include "lcd.h"
#include "uart.h"

/* Sends Password to Control ECU and returns whether password is correct or not */
boolean checkPassword(uint8* pass);

/* Gets password from user using keypad and saves it to passed array */
void getPassword(uint8* pass);

/* Called when 3 consecutive wrong passwords are entered */
void soundAlarm();

/* Called on first startup to configure new password */
void firstStartup();

/* Called when the user chooses the "open door" option in main menu */
void openDoor();

/* Called when the user chooses the "change password" option in main menu */
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


/* -Sends passed password to Control ECU using UART
 * -Waits for Control ECU response regarding password
 * -returns whether password is correct or not
 */
boolean checkPassword(uint8* pass) {
	UART_sendByte(0x05); /* Send command to Control ECU requesting a password check */
	UART_sendString(pass); /* Send password to Control ECU */
	return UART_receiveByte(); /* Receive password check result from Control ECU and return it to caller */
}

/* Gets password from user using keypad and saves it to passed array */
void getPassword(uint8* pass) {
	uint8 key;
	for (int i = 0; i < 16 /* Maximum password length */; i++) {
		key = KEYPAD_getPressedKey();
		_delay_ms(500);
		if (key == 13) { /* If the user presses Enter, add 0 to signify end of password and break */
			pass[i] = 0;
			break;
		}
		LCD_displayCharacter('*'); /* Display asterisks to user to signify key presses recorded */
		pass[i] = key + 48; /* Store the numbers in ASCII format */
	}
}

/* -Called when 3 consecutive wrong passwords are entered
 * -Sends command to Control ECU signaling intruder alert
 * -Prints "Intruder Alert!" to Screen
 * -Halts Operation of HMI ECU until Control ECU signals that 60 seconds have passed and Alarm is off
 */
void soundAlarm() {
	UART_sendByte(0xFF); /* Send command to Control ECU signaling intruder alert */

	/* Print "Intruder Alert!" to Screen */
	LCD_clearScreen();
	LCD_displayString("Intruder Alert!");

	/* Halt Operation of HMI ECU until Control ECU signals that 60 seconds have passed and Alarm is off */
	while (UART_receiveByte() != 0x00);
}

/* -Called on first startup to configure new password
 * -Gets password from User
 * -Sends Password to Control ECU
 */
void firstStartup() {
	uint8 pass1[16];
	uint8 pass2[16];

	/* Get Password from User */
	LCD_displayString("New Password:");
	LCD_moveCursor(1, 0);
	getPassword(pass1);

	/* Get password a second time from the user */
	LCD_clearScreen();
	LCD_displayString("Again:");
	LCD_moveCursor(1, 0);
	getPassword(pass2);

	/* Check for password mismatch */
	for (int i = 0; i < 16; i++) {
		if (pass1[i] != pass2[i]) {
			LCD_clearScreen();
			LCD_displayString("Mismatch");
			_delay_ms(2000);
			firstStartup(); /* call function again to re-get password */
			return;
		}
		if (pass1[i] == 0) break;
	}

	/* Send password to Control ECU */
	UART_sendString(pass1);
}

/* -Called when the user chooses the "open door" option in main menu
 * -Gets password form User
 * -Checks password
 * -Waits for Control ECU to open door if password is correct
 * -Increments wrong password counter and calls itself again if password is wrong
 * -Sounds alarm if wrong password counter reaches 3
 */
void openDoor() {
	uint8 pass[16];

	/* Wrong password counter, Static so that it can be used when the function calls itself recursively */
	static uint8 wrongCount = 0;

	/* Get password from User */
	LCD_clearScreen();
	LCD_displayString("Password:");
	LCD_moveCursor(1,0);
	getPassword(pass);

	/* Check if Password is correct */
	if (checkPassword(pass) == 0x01) {
		/* Wait for Control ECU to execute door opening and re-closing procedure and update LCD accordingly */

		/* Wait for Control ECU to finish opening the door */
		LCD_clearScreen();
		LCD_displayString("Opening Door");
		while (UART_receiveByte() != 0x0C);

		/* Wait for Control ECU to start re-closing the door */
		LCD_clearScreen();
		LCD_displayString("Door Open");
		while (UART_receiveByte() != 0x0C);

		/* Wait for Control ECU to finish closing the door */
		LCD_clearScreen();
		LCD_displayString("Closing Door");
		while (UART_receiveByte() != 0x0C);

		/* Reset wrong password counter for re-use next time */
		wrongCount = 0;
	}
	else {
		/* Print Error message to user indicating Wrong Password */
		LCD_clearScreen();
		LCD_displayString("Wrong Password!");

		/* Increment wrong password counter */
		wrongCount++;

		/* Check for wrong password count */
		if (wrongCount == 3) {
			soundAlarm(); /* Sound the Alarm if wrong password count reaches 3 */
			wrongCount = 0; /* Reset wrong password count for re-use next time */
		}
		else {
			_delay_ms(2000); /* Wait for2 seconds for the user to read the error message on LCD */
			openDoor(); /* Recall the function recursively to repeat password entering procedure */
		}
	}
}

/* Called when the user chooses the "change password" option in main menu */
void changePassword() {
	/* Arrays to save password */
	uint8 oldPass[16];
	uint8 newPass1[16];
	uint8 newPass2[16];

	/* static counter and flag to track function recursive calls */
	static uint8 wrongCount = 0;
	static uint8 correctOldPass = 0;

	/* Check for old password only once in a single call from main */
	if (!correctOldPass) {
		LCD_clearScreen();
		LCD_displayString("Old Password:");
		LCD_moveCursor(1,0);
		getPassword(oldPass);
		UART_sendByte(0x08); /* Send message to ECU requesting a password check and change */
		UART_sendString(oldPass);
		/* Set flag indicating that old password has been entered correctly */
		correctOldPass = UART_receiveByte();
	}

	if (correctOldPass) {
		/* get new password from user if old password is correct */
		LCD_clearScreen();
		LCD_displayString("New Password:");
		LCD_moveCursor(1,0);
		wrongCount = 0;
		getPassword(newPass1);

		/* Get password a second time from the user */
		LCD_clearScreen();
		LCD_displayString("Again:");
		LCD_moveCursor(1, 0);
		getPassword(newPass2);

		/* Check for password mismatch */
		for (int i = 0; i < 16; i++) {
			if (newPass1[i] != newPass2[i]) {
				LCD_clearScreen();
				LCD_displayString("Mismatch");
				_delay_ms(2000);
				changePassword(); /*call function again to re-get password */
				return;
			}
			if (newPass1[i] == 0) break;
		}

		/* Send new password to control ECU */
		UART_sendString(newPass1);
	}
	else {
		/* Print Error message to user indicating Wrong Password */
		LCD_clearScreen();
		LCD_displayString("Wrong Password!");

		/* Increment wrong password counter */
		wrongCount++;


		if (wrongCount == 3) {
			soundAlarm(); /* Sound the Alarm if wrong password count reaches 3 */
			wrongCount = 0; /* Reset wrong password count for re-use next time */
		}
		else {
			_delay_ms(2000); /* Wait for 2 seconds for the user to read the error message on LCD */
			changePassword(); /* Recall the function recursively to repeat password entering procedure */
		}
	}

	correctOldPass = 0; /* Reset flag on end of the first call from main for re-use */
}



