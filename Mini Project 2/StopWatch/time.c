#include "time.h"

void incrementTime(time* t) {
	t -> seconds++;
	if (t -> seconds == 60) {
		t -> seconds = 0;
		t -> minutes++;
	}
	if (t -> minutes == 60) {
		t -> minutes = 0;
		t -> hours++;
	}
	if (t -> hours == 100) t -> hours = 0; //reset after reaching 100 hours
}
