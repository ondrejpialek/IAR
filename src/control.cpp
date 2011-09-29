#include "control.h"
#include "MotorControl.h"
#include <unistd.h>

void initControl() {
	initMotors();
}

void forward(int distance) {
	setAcceleration(0, 50.00);
	setVelocity(0, 100.00);
	setAcceleration(1, 50.00);
	setVelocity(1, -100.00);
	sleep(distance);
}

//degrees relative to current direction
void turn(int degrees) {
	int modifier = 1;
	if (degrees < 0)
		modifier = -1;

	setAcceleration(0, 50.00);
	setVelocity(0, 100.00 * modifier);
	setAcceleration(1, 50.00);
	setVelocity(1, 100.00 * modifier);
    sleep((degrees * modifier) / 100);
}

void stop() {
	setAcceleration(0, 0.00);
	setVelocity(0, 0.00);
	setAcceleration(1, 0.00);
	setVelocity(1, 0.00);
}

void releaseControl() {
	closeMotors();
}

