#include "distanceSensors.h"

void initDistance() {
	initDistanceSensors();
}

int getFrontDistance() {
	return 4800 / ((readings[1]/4.095) - 20);
}
