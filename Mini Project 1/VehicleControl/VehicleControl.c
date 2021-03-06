#include <stdio.h>

#define WITH_ENGINE_TEMP_CONTROLLER 1

typedef enum BinaryState {ON, OFF} BinaryState;

typedef struct {
	BinaryState state;
	#if(WITH_ENGINE_TEMP_CONTROLLER)
	BinaryState tempController;
	int temp;
	#endif
}engineType;

typedef struct {
	BinaryState state;
	int temp;
}acType;

int speed = 0;
engineType engine = {OFF, OFF, 125};
acType ac = {OFF, 20};

void printState() {
	printf("Engine: "); (engine.state == ON)?printf("ON\n"):printf("OFF\n");
	printf("AC: "); (ac.state == ON)?printf("ON\n"):printf("OFF\n");
	printf("Vehicle Speed: %d\n", speed);
	printf("Room Temperature: %d\n", ac.temp);
	#if(WITH_ENGINE_TEMP_CONTROLLER)
	printf("Engine Temperature Controller State: "); (engine.tempController == ON)?printf("ON\n"):printf("OFF\n");
	printf("Engine Temperature: %d\n", engine.temp);
	#endif
	printf("\n");
}

#if(WITH_ENGINE_TEMP_CONTROLLER)
void setEngineTemp(int temp) {
	if (temp < 100) {
		engine.tempController = ON;
		engine.temp = 125;
	}
	else if (temp > 150) {
		engine.tempController = ON;
		engine.temp = 125;
	}
	else {
		engine.tempController = OFF;
		engine.temp = temp;
	}
	printState();
}
#endif

void setRoomTemp(int temp) {
	if (temp < 10) {
		ac.state = ON;
		ac.temp = 20;
	}
	else if (temp > 30) {
		ac.state = ON;
		ac.temp = 20;
	}
	else {
		ac.state = OFF;
		ac.temp = temp;
	}
	printState();
}

void setSpeed(char trafficLight) {
	if(trafficLight == 'G') speed = 100;
	else if(trafficLight == 'R') speed = 0;
	else if(trafficLight == 'O') {
		speed = 30;
		ac.state = ON;
		ac.temp = (int)(((float)ac.temp)*(5.0/4.0) + 1); //temp = temp * 5/4 + 1
		#if(WITH_ENGINE_TEMP_CONTROLLER)
		engine.tempController = ON;
		engine.temp = (int)(((float)engine.temp*(5.0/4.0)) + 1); //temp = temp * 5/4 + 1
		#endif
	}
	printState();
}

char engineMenu() { //returns 1 if c (Quit) was chosen and 0 otherwise
	char input;
	printf("a. Turn on the vehicle engine\n");
	printf("b. Turn off the vehicle engine\n");
	printf("c. Quit the system\n");
	scanf(" %c", &input);
	if(input == 'c') return 1;
	else if(input == 'a') engine.state = ON;
	else if(input == 'b') engine.state = OFF;
	return 0;
}

void sensorMenu() {
	char inputChar;
	int inputInt;
	printf("a. Turn off engine\n");
	printf("b. Set the traffic light color\n");
	printf("c. Set the room temperature\n");
	#if(WITH_ENGINE_TEMP_CONTROLLER)
	printf("d .Set the engine temperature\n");
	#endif
	scanf(" %c", &inputChar);
	if (inputChar == 'a') engine.state = OFF;
	else if(inputChar == 'b') {
		printf("Set traffic light color(R,O,G): ");
		scanf(" %c", &inputChar);
		setSpeed(inputChar);
	}
	else if(inputChar == 'c') {
		printf("Set the room temperature: ");
		scanf(" %d", &inputInt);
		setRoomTemp(inputInt);
	}
	#if(WITH_ENGINE_TEMP_CONTROLLER)
	else if(inputChar == 'd') {
		printf("Set the engine temperature: ");
		scanf(" %d", &inputInt);
		setEngineTemp(inputInt);
	}
	#endif
}

int main(void) {
	setbuf(stdout, NULL);
	while(1) {
		if(engine.state == OFF) {
			if (engineMenu()) break; //if quit was chosen, engineMenu returns 0
		}
		else if(engine.state == ON) {
			sensorMenu();
		}
	}
	return 0;
}
