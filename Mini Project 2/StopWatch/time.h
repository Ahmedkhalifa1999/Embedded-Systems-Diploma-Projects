typedef struct {
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hours;
}time;

void incrementTime(time* t);  //increments given time by 1 seconds (variable passed by reference)
