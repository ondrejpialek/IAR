#include <stdio.h>
#include <time.h>
#include <float.h>

#include <libpowerbutton.h>
#include <phidget21.h>

#include "control.h"
#include "distance.h"
/*#include <sys/time.h>*/

int msleep(unsigned long milisec)
{
    struct timespec req = { 0 };
    time_t sec = (int) (milisec / 1000);
    milisec = milisec - (sec * 1000);
    req.tv_sec = sec;
    req.tv_nsec = milisec * 1000000L;
    while (nanosleep(&req, &req) == -1)
	continue;
    return 1;
}

int main(int argc, char *argv[])
{
    double wasWhisker = 0;
  
    initControl();
    initDistance();

    msleep(1000);

    int pwr = power_button_get_value();

    timespec current;
    timespec old;

    clock_gettime(CLOCK_MONOTONIC, &current);
    while (pwr == power_button_get_value()) {
	printf("-------------------------------\n");
	old = current;
	clock_gettime(CLOCK_MONOTONIC, &current);

	double diff = (current.tv_sec - old.tv_sec) + ((current.tv_nsec - old.tv_nsec) / NANOSECONDS_PER_SECOND);

	printf("T: %f\n", diff);
	
	controlTick(diff);

	int left = getLeftDistance();
	int right = getRightDistance();
	
	printf("L:%8d R:%8d\n", left, right);

	if (getBackWhisker() && getFrontWhisker()) {
	    double mod = 1;
	    if (left > right) {
		mod = -1;
	    }
	    turn(mod * 90);
	    printf("WW: PANIC\n");
	} else if (getBackWhisker()) {
	    wasWhisker = 2;
	    printf("BW: forward\n");
	    move(20);
	} else if (getFrontWhisker()) {
	    wasWhisker = 2;
	    printf("FW: back\n");
	    move(-20);
	} else if (wasWhisker > 0) {
	    wasWhisker -= diff;
	    printf("WT: turning from whisker\n");
	    turn(45);
	} else if ((left < 8 || left > 500) && (right < 8 || right > 500)) {
	    move(20);
	    printf("CLEAR: forward\n");
	} else if (left > 8 && left < 45) {
	    turn(-50);
	    printf("LE: right\n");
 	} else if (right > 8 && right < 45) {
	    turn(50);
	    printf("RG: left\n");
	} else {
	    move(20);
	    printf("EL: forward\n");
	}
	printf("\n");
	msleep(150);
    }

    stop();
    closeDistance();
    releaseControl();
    power_button_reset();

    return 0;
};
