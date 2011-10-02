#include "distanceSensors.h"

void initDistance() {
	initDistanceSensors();
}

int getFrontDistance() {
	return 4080 / (readings[1] - 20);
}
