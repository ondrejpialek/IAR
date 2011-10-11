#include <stdio.h>
#include <time.h>
#include <float.h>

#include <libpowerbutton.h>
#include <phidget21.h>

#include "control.h"
#include "sensing.h"
#include "strategy.h"


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
    Sensing* sensing = new Sensing();    
    Control* control = new Control();    
    
    Strategy* strategies [] = { new FindSiteStrategy(sensing, control), new HitButtonStrategy(sensing, control) };
    int STRATEGIES_COUNT = 2;
    

    int pwr = power_button_get_value();
    while(pwr == power_button_get_value()) {
        msleep(500);
    }
    sensing->adjustFloorLevel();   

    
    timespec current;
    timespec old;
    clock_gettime(CLOCK_MONOTONIC, &current);
    
    pwr = power_button_get_value();
    Strategy* oldStrategy = 0;
    while (pwr == power_button_get_value()) {
        printf("-------------------------------\n");
        
        double bestUtility = -1;
        int strategyIndex = 0;
        Strategy* strategy = 0;
        for (int i = 0; i < STRATEGIES_COUNT; i++) {
            Strategy* s = strategies[i];
            double util = s->getUtility();
            if (util > bestUtility) {
                strategy = s;
                bestUtility = util;
                strategyIndex = i;
            }
        }
/*
printf("R: %d\n", sensing->getRightLight());

printf("L: %d\n", sensing->getLeftLight());*/

        old = current;
        clock_gettime(CLOCK_MONOTONIC, &current);  
        double diff = (current.tv_sec - old.tv_sec) + ((current.tv_nsec - old.tv_nsec) / NANOSECONDS_PER_SECOND);
        
        printf("T: %f, S: %d\n", diff, strategyIndex);
        
        control->controlTick(diff);
        strategy->step(diff, strategy != oldStrategy);
        oldStrategy = strategy;
        
        msleep(70);
    }
    
    control->stop();
    msleep(100);
    
    for (int i = 0; i < STRATEGIES_COUNT; i++) {
        delete strategies[i];
    }
    
    delete sensing;
    delete control;
    
    power_button_reset();
    
    return 0;
};
