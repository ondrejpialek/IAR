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
    initControl();
    initDistance();

    msleep(1000);

    int pwr = power_button_get_value();

    timespec current;
    timespec old;

    clock_gettime(CLOCK_MONOTONIC, &current);
    
    turn(45);
    
    int i = 0;
    while (i++ < 5) {
        old = current;
        clock_gettime(CLOCK_MONOTONIC, &current);

        double diff = (current.tv_sec - old.tv_sec) + ((current.tv_nsec - old.tv_nsec) / NANOSECONDS_PER_SECOND);

        printf("T: %f\n", diff);
        
        controlTick(diff);

        int left = getLeftDistance();
        int right = getRightDistance();
        
        printf("L:%8d R:%8d\n", left, right);	
        
        msleep(100);
    }

    releaseControl();
    power_button_reset();

    return 0;
};
