#include <libpowerbutton.h>
#include <stdio.h>
#include <phidget21.h>
#include "control.h"
#include "distance.h"
#include <sys/time.h>

int main (int argc, char *argv[])
{
	initControl();
	initDistance();

	sleep(2);

	timespec current;
	timespec old;

	clock_gettime(CLOCK_MONOTONIC, &current);
	while(true) {
		old = current;
		double diff = (current.tv_nsec - old.tv_nsec) / 1000000;
		clock_gettime(CLOCK_MONOTONIC, &current);

		int distance = getFrontDistance();
		printf("Distance: %d\n", distance);
/*
		if (distance < 100) {
			move(-10);
		} if (distance < 200) {
			turn(138);
		} else {
			move(10);
		}*/

		sleep(1);
	}
	return 0;

	/*initControl();
	printf("Max acceleration, motor 0: %f",getMaxAcceleration(0));
	printf("Max acceleration, motor 1: %f",getMaxAcceleration(1));

	int start = power_button_get_value();
    printf("Button pressed %i times.\n",power_button_get_value());
    int current = power_button_get_value();
    while(current - start <5)
    {
    	if (current != power_button_get_value()) {
    		printf("Button pressed %i times.\n",current+1);
    		forward(2);
    		turn(-180);
    		forward(3);
    		turn(90);
    		forward(1);
    		stop();
    	}

    	current = power_button_get_value();
    	sleep(1);
    }

    releaseControl();
    power_button_reset();
    return 0;*/
};
