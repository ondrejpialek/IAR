#include <cmath>

#define PI 3.14159265

#include "strategy.h"

Strategy::Strategy(Sensing* sensing, Control* control) {
    this->sensing = sensing;
    this->control = control;
}

double FindSiteStrategy::getUtility() {
    double utility = 0.9;
    if (sensing->isLeftOnBlack() || sensing->isRightOnBlack()) {
        utility = 0;
    }
    return utility;
}

void FindSiteStrategy::step(double delta, bool firstRun) {
    int left = sensing->getLeftDistance();
    int right = sensing->getRightDistance();
    bool fWhisker = sensing->getFrontWhisker();
    bool bWhisker = sensing->getBackWhisker();
    
    #ifndef SILENT_STRATEGY
    printf("L:%8d R:%8d\n", left, right);
    #endif
    
    if (bWhisker && fWhisker) {
        double mod = 1;
        if (left > right) {
            mod = -1;
        }
        control->turn(mod * 90);
        #ifndef SILENT_STRATEGY
        printf("WW: PANIC\n");
        #endif
    } else if (bWhisker) {
        wasWhisker = 0.8;
        #ifndef SILENT_STRATEGY
        printf("BW: forward\n");
        #endif
        control->move(20);
    } else if (fWhisker) {
        wasWhisker = 0.8;
        #ifndef SILENT_STRATEGY
        printf("FW: back\n");
        #endif
        control->move(-20);
    } else if (wasWhisker > 0) {
        wasWhisker -= delta;
        #ifndef SILENT_STRATEGY
        printf("WT: turning from whisker\n");
        #endif
        control->turn(45);
    } else if ((left < 6 || left > 500) && (right < 6 || right > 500)) {
        control->move(20);
        #ifndef SILENT_STRATEGY
        printf("CLEAR: forward\n");
        #endif
    } else if (left > 6 && left < 20) {
        control->turn(-50);
        #ifndef SILENT_STRATEGY
        printf("LE: right\n");
        #endif
    } else if (right > 6 && right < 20) {
        control->turn(50);
        #ifndef SILENT_STRATEGY
        printf("RG: left\n");
        #endif
    } else {
        control->move(20);
        #ifndef SILENT_STRATEGY
        printf("EL: forward\n");
        #endif
    }    
}

double HitButtonStrategy::getUtility() {
    double utility = 0;
    
    if (currentTask > Align) {
        utility = 1;
    }
    
    if (sensing->isLeftOnBlack() || sensing->isRightOnBlack()) {
        utility = 1;
    }
    return utility;
}

void HitButtonStrategy::step(double delta, bool firstRun) {
    if (directionProtection > 0) {
        directionProtection -= delta;    
    }
    
    if (firstRun)
        reset();
    
    switch (currentTask) {
        case Align: {
            int l = sensing->getLeftDistance();
            int r = sensing->getRightDistance();
            
            if (l == 0 || r == 0) {
                control->turnSlow(turnDirection * 30);
                break;
            }
            
            int diff = (l - r) * -1;
            
            if (fabs(diff) > 3) {
                control->turnSlow(diff*4);
            } else {
                control->stop();
                currentTask = GetWhiskerContact;
                                
                printf("Next: %d\n", currentTask);
            }
            break;
        }

        case GetWhiskerContact: {
            if (sensing->getFrontWhisker()) {
                control->stop();
                currentTask = FindCentre;
                
                directionProtection = 0;
                distanceCount = 0;
                
                printf("Next: %d\n", currentTask);                
            } else {
                control->moveSlow(10);
            }
            break;
        }

        case FindCentre: {
            int l = sensing->getLeftDistance();
            int r = sensing->getRightDistance();
            
            if (l == 0 || r == 0) {
                control->stop();
                turnDirection = turnDirection * -1;
                currentTask = Align;
                printf("Wrong direction, realign and try again");
                break;               
            }
            
            int distance;
            if (turnDirection == 1) {
                distance = r;
            } else {
                distance = l;
            }
            
            
            double deviation = getDeviation(distance);
            printf("Deviation: %f\n", deviation); 
            
            if ((deviation > 0.6) && (deviation < 0.90)) {
                control->stop();
                currentTask = PushTheButton;
                printf("Next: %d\n", currentTask);
                break;
            }
            
            if (deviation > 1.1) {
                control->stop();
                turnDirection = turnDirection * -1;
                currentTask = Align;
                printf("Something phishy, realign and try again");
                break;                    
            }
            
            control->turnSingle(10 * turnDirection);
            
            /*double l = sensing->getLeftDistance();
            double r = sensing->getRightDistance();
            printf("L: %f R: %f\n", l, r);       
            
            double ratio = (l + 1) / (r + 1);
            if (ratio > 1)
                ratio = 1 / ratio;
            printf("R: %f\n", ratio);                    
            
            if (0.75 <= ratio) {            
                control->stop();
                currentTask = PushTheButton;
                printf("Next: %d\n", currentTask);                    
            } else {
                if (directionProtection <= 0) {
                    turnDirection = turnDirection * -1;
                    directionProtection = 2;
                }
                control->turnSlow(10 * turnDirection);
            }*/
            break;
        }
        case PushTheButton: {
            printf("PUSH THE BUTTON!\n");
            control->stop(); 
            break;
        }
    }
}

double HitButtonStrategy::getDeviation(int distance) {    
    if (distanceCount > 1)
        if ((distances[distanceCount-1] -2) >= distance)
            return 6;//error
            
            
    
    distances[distanceCount] = distance;
    distanceCount++;
    printf("Distances[%d]: %f\n", distanceCount-1, distances[distanceCount-1]);
    
    if (distanceCount <= 2)
        return 0;
    
    double alpha = 0;
    
    double perpendicularLineDistance = distances[0];
    double previousDistance = distances[0];
    double count = 0;
    for (int i=1; i< distanceCount-1; i++) {
        double d = distances[i];
        if (previousDistance <= d) {        
            double a = (acos(perpendicularLineDistance/d) / i) * 180.0 / PI;
            printf("for d: %f, a: %f\n", d, a);
            if (a == 0) {
                a = 0.8;  
            }
            alpha += a; 
            count++;
            previousDistance = d;
        }
    }
    
    if (count == 0) {
        if (distanceCount > 10) {
            return 69; //error
        }
    }

    alpha /= count;        

    
    printf("alpha: %f\n", alpha);
    
    /*for (int i=1; i< distanceCount-1; i++) {
        double betaStar = 90+ i * alpha;
        double gamma = 180 - alpha - betaStar;
        double expectedDistance = (distances[i] * betaStar) / gamma;
        double error = distances[i+1] / expectedDistance;
        if (error > 0.05) {
            return i;
        }
    }*/
    
    int i = distanceCount - 2;
    double betaStar = 90+ i * alpha;
    double gamma = 180 - alpha - betaStar;
    double expectedDistance = (distances[i] * betaStar) / gamma; //floor(+ 0.5)
    double error = distances[i+1] / expectedDistance;
    printf("Expected: %f, actual: %f\n", expectedDistance, distances[i+1]);
    return error;
} 

void HitButtonStrategy::reset() {
    printf("THIS! IS! REEEEESEEEET!\n");
    control->stop();
    currentTask = Align;
    cameFromLeft = sensing->isLeftOnBlack() && !sensing->isRightOnBlack();
    if (cameFromLeft) {
        printf("LEFT!!\n");
        turnDirection = 1;
    } else {
        printf("RIGHT!!\n");
        turnDirection = -1;
    }
}

