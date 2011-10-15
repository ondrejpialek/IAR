#include <cmath>

#define PI 3.14159265
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

#include "strategy.h"

Strategy::Strategy(Sensing* sensing, Control* control) {
    this->sensing = sensing;
    this->control = control;
}

double FindSiteStrategy::getUtility() {
    double utility = 0.8;
    if (sensing->isLeftOnBlack() || sensing->isRightOnBlack()) {
        utility = 0;
    }
    return utility;
}

void FindSiteStrategy::step(double delta, bool firstRun) {
    int left = sensing->getLeftDistance();
    int right = sensing->getRightDistance();

    
    #ifndef SILENT_STRATEGY
    printf("L:%8d R:%8d\n", left, right);
    #endif
    
    if (sensing->getLeftBumper() || sensing->getRightBumper()) {
        control->move(-20);
        wasBumper = 0.8;
        #ifndef SILENT_STRATEGY
        printf("BUM: back!\n");
        #endif
    } else if (sensing->getLeftWhisker()) {
        control->turnSingle(-30);
        #ifndef SILENT_STRATEGY
        printf("LW: turn single right\n");
        #endif
    } else if (sensing->getRightWhisker()) {
        control->turnSingle(30);
        #ifndef SILENT_STRATEGY
        printf("RW: turn single left\n");
        #endif
        control->move(20);
    } else if (wasBumper > 0) {
        wasBumper -= delta;
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
        utility = 0.9;
    }
    
    if (sensing->isLeftOnBlack() || sensing->isRightOnBlack()) {
        utility = 0.9;
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
            printf("ALIGNING!\n");
            int l = sensing->getLeftDistance();
            int r = sensing->getRightDistance();
            
            printf("L:%8d R:%8d\n", l, r);   
            
            if (l == 0 || r == 0) {
                control->turnSlow(turnDirection * 30);
                break;
            }
            
            int diff = (l - r) * -1;
            if (!(sensing->getLeftBumper() || sensing->getRightBumper())) {
                if (fabs(diff) > 3) {
                    control->turnSlow(diff*4);
                } else if (l > 10) {
                    control->moveSlow(10);
                } else {
                    control->stop();
                    
                    maxDistanceWhileTurning = 0;
                    currentTask = TurnToWall;
                    
                    printf("Next: %d\n", currentTask);                  
                }
            } else {
                control->stop();
                
                maxDistanceWhileTurning = 0;
                currentTask = TurnToWall;
                                
                printf("Next: %d\n", currentTask);
            }
            break;
        }

        case TurnToWall: {
            printf("TURNING TO WALL!\n");
            /*
            int l = sensing->getLeftDistance();
            int r = sensing->getRightDistance();
            
            int max = MAX(l, r);
            if (max < (maxDistanceWhileTurning / 2)) {
                currentTask = Align;
                printf("Next: %d\n", currentTask); 
                break;
            }
            
            maxDistanceWhileTurning = MAX(maxDistanceWhileTurning, max);
            */
            if (sensing->getLeftWhisker() || sensing->getRightWhisker()) {
                control->stop();
                turningToButton = 0;
                turningProtection = 0;
                
                currentTask = PushTheButton;
                printf("Next: %d\n", currentTask);  
            } else if (sensing->getLeftBumper() || sensing->getRightBumper()) {
                control->moveSlow(-5);
            } else {
                control->turnSlow(turnDirection * 10);
            }
            
            break;
        }

        case PushTheButton: {
            printf("PUSH THE BUTTON!\n");
            
            if (turningProtection > 0)
                turningProtection -= delta;
            
            if (turningToButton > 0)
                turningToButton -= delta;
   
            if (sensing->getRightLight() > 200) {
                printf("LIGHT: %d\n", sensing->getRightLight());
                sensing->getRightLight();
                victoryBacking = 2;
                currentTask = VictoryDance;
                printf("Next: %d\n", currentTask);
            } else  if (!(sensing->isLeftOnBlack() || sensing->isRightOnBlack())) {
                control->stop();
                currentTask = BackTrack;
                printf("Next: %d\n", currentTask);                  
            } else if (sensing->getLeftBumper() || sensing->getRightBumper()) {
                printf("BUMPER\n");
                if (bumperReaction) {
                    control->turnSlow(10 * turnDirection * -1);
                    bumperReaction = !bumperReaction;
                } else {
                    control->moveSlow(-10);
                    bumperReaction = !bumperReaction;
                }
            } else if (turningToButton > 0) {
                printf("TTB: %f\n", turningToButton);
                control->turnSlow(10 * turnDirection * -1);
                turningProtection = 0.5;
            } else if ((turningProtection <= 0)  && !(sensing->getLeftWhisker() || sensing->getRightWhisker())) {
                printf("LOST WHISKER\n");
                control->turnSlow(10 * turnDirection * -1);
                turningToButton = 0.2;
            } /*else if ((l < 15) && (r > 15)) {
                control->turnSlow(15);
            } else if ((r < 15)  && (l > 15)) {
                control->turnSlow(-15);
            } */else {
                printf("FORWARD\n");
                control->moveSlow(10);
            }
            
            break;
        }
        
        case BackTrack: {
            printf("BACKTRACKING!\n");
            
            if (!(sensing->isLeftOnBlack() && sensing->isRightOnBlack())) {
                control->move(turnDirection * 20);
            } else {
                turnDirection *= -1;
                currentTask = Align;
                printf("Next: %d\n", currentTask);
            }
            break;
        }
        
        case VictoryDance: {
            printf("VDANCE!\n");
            if (victoryBacking > 0) {
                control->move(-10);     
                victoryBacking -= delta;
                if (victoryBacking <= 0) {
                    dancing = 5;
                }
            } else if (dancing > 0) {
                dancing -= delta;
                control->turn(10);
            } else {
                currentTask = Align;
                printf("Next: %d\n", currentTask);                
            }
            break;
        }
    }
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

