#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include <libpowerbutton.h>
#include <phidget21.h>

#include "control.h"
#include "motorControl.h"


#define NANOSECONDS_PER_SECOND 1E9

int main (int argc, char *argv[])
{
	initControl();
	
	printf("LEFT:  %s\n", argv[1]);
	printf("RIGHT: %s\n", argv[2]);
	
	LEFT_MOTOR_MODIFIER = atof(argv[1]);
	RIGHT_MOTOR_MODIFIER = atof(argv[2]);

	printf("LEFT:  %f\n", LEFT_MOTOR_MODIFIER);
	printf("RIGHT: %f\n", RIGHT_MOTOR_MODIFIER);
	
	timespec current;
	timespec old;

	move(20);
	clock_gettime(CLOCK_MONOTONIC, &current);
	int i = 0;
	while(i < 3) {
		old = current;
		clock_gettime(CLOCK_MONOTONIC, &current);
		
		double diff = ( current.tv_sec - old.tv_sec ) + ((current.tv_nsec - old.tv_nsec) / NANOSECONDS_PER_SECOND);
		

		printf("diff: %f\n", diff);
		
		controlTick(diff);
		
		sleep(1);
		i++;
	}
	
	stop();
	
	releaseControl();
	
	return 0;
};
