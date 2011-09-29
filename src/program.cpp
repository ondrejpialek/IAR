#include <libpowerbutton.h>
#include <stdio.h>
#include <phidget21.h>
#include <control.h>

int main (int argc, char *argv[])
{
	initControl();
	printf("Max acceleration, motor 0: %f",getMaxAcceleration(0));
	printf("Max acceleration, motor 1: %f",getMaxAcceleration(1));

	int start = power_button_get_value();
    printf("Button pressed %i times.\n",power_button_get_value());
    int current = power_button_get_value();
    while(current - start <5)
    {
    	if (current != power_button_get_value()) {
    		printf("Button pressed %i times.\n",current+1);
    		forward(5);
    		turn(-180);
    		forward(10);
    		turn(90);
    		forward(3);
    		stop();
    	}

    	current = power_button_get_value();
    	sleep(1);
    }

    releaseControl();
    power_button_reset();
    return 0;
};
