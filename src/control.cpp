#include "control.h"
#include "motorControl.h"
#include <unistd.h>

double remaining;

void initControl() {
	initMotors();
}

void move(int distance) {
	int modifier = 1;
	if (distance < 0)
		modifier = -1;

	setAcceleration(0, 50.00);
	setVelocity(0, -1 * modifier * 100.00);
	setAcceleration(1, 50.00);
	setVelocity(1, modifier * 100.00);
	remaining = (modifier * distance) / 10;
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
	remaining = (degrees * modifier) / 500;
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

void controltick(double diff) {
	remaining -= diff;
	if (remaining < 0) {
		stop();
	}
}
