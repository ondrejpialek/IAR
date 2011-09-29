#include <libpowerbutton.h>
#include <stdio.h>
#include <phidget21.h>
#include "MotorControl.h"

int main (int argc, char *argv[])
{
	init();
	printf("Max acceleration, motor 0: %f",getMaxAcceleration(0));
	printf("Max acceleration, motor 1: %f",getMaxAcceleration(1));

	int start = power_button_get_value();
    printf("Button pressed %i times.\n",power_button_get_value());
    int current = power_button_get_value();
    while(current - start <5)
    {
    	if (current != power_button_get_value()) {
    		printf("Button pressed %i times.\n",current+1);
    		setAcceleration(0, 50.00);
    		setVelocity(0, 100.00);
    		setAcceleration(1, 50.00);
    		setVelocity(1, 100.00);
    	    sleep(2);
    		setAcceleration(0, 0.00);
    		setVelocity(0, 0.00);
    		setAcceleration(1, 0.00);
    		setVelocity(1, 0.00);
    	}

    	current = power_button_get_value();
    	sleep(1);
    }
    power_button_reset();
    close();
    return 0;
};
