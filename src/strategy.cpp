#include <cmath>

#define PI 3.14159265
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

#include "strategy.h"

Strategy::Strategy(Sensing* sensing, Control* control, ServoControl* servo) {
    this->sensing = sensing;
    this->control = control;
    this->servo = servo;
}

double FindSiteStrategy::getUtility() {
    double utility = 0.8;
    if (sensing->isLeftOnBlack() || sensing->isRightOnBlack()) {
        utility = 0;
    }
    return utility;
}

void FindSiteStrategy::step(double delta, bool firstRun) {
    if (firstRun) { servo->doScan(); }
  
    int top = sensing->getTopDistance();
    int bottom = sensing->getBottomDistance();
    int sonar = sensing->getSonarDistance();
    
    #ifndef SILENT_STRATEGY
    printf("L:%8d R:%8d\n", left, right);
    #endif
    
    if (sensing->getLeftBumper() || sensing->getRightBumper()) {
        control->move(-20);
        wasBumper = 0.6;
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
    } else if (sonar < 25) {
	control->turnSingle(30);
        #ifndef SILENT_STRATEGY
        printf("SONAR: turn single left\n");
        #endif
    } else if (sonar > 140) {
	control->turnSingle(180);
	#ifndef SILENT_STRATEGY
        printf("SONAR: facing outside\n");
        #endif
    } else if ((top < 6 || top > 500) && (bottom < 6 || bottom > 500)) {
        control->move(20);
        #ifndef SILENT_STRATEGY
        printf("CLEAR: forward\n");
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
       
    if ((darkAreaCooldown > 0) || sensing->isLeftOnBlack() || sensing->isRightOnBlack()) {
        utility = 0.9;
    }
    return utility;
}

void HitButtonStrategy::step(double delta, bool firstRun) {    
    if (firstRun)
        reset();
    
    darkAreaCooldown -= delta;
    
    if (sensing->isLeftOnBlack() || sensing->isRightOnBlack())
	darkAreaCooldown = 2;
    
    if (currentTask != Align) {
        qualityAssurance = 3;    
    }
    
    switch (currentTask) {
        case Align: {
            
            printf("ALIGNING!\n");
            
            if (sensing->getLeftBumper() || sensing->getRightBumper()) {
                printf("HIT BUMPERS!\n");
                improveTimer = 0.5;
                currentTask = MoveToCentre;
            }
            else if (sensing->getRightWhisker() || sensing->getLeftWhisker()) {
                printf("HIT WHISKERS!\n");
                improveTimer = 0.5;
                currentTask = MoveToCentre;
            }
            else {
                if (directionProtection > 0) {
                    directionProtection -= delta;
                }
                
                int t = sensing->getTopDistance();
                int b = sensing->getBottomDistance();
                
                printf("D: t:%d, b:%d\n", t, b);
                
                if (((b == 0) || (b > 100)) && (sensing->getSonarDistance() > 50)) {
                    printf("BOTTOM SENSOR TRIGGER\n");
                    if (directionProtection > 0) {  
                        control->turnSingle(turnDirection*30);
                    } else {
                        printf("DIRECTION CHANGE!\n");
                        turnDirection *= -1;
                        directionProtection = 1.5;
                        control->turnSingle(turnDirection*30);
                    }
                }
                else if ((((t > b+5) || (t == 0)) && (directionProtection <= 0)) && (sensing->getSonarDistance() < 70)) {
                    printf("SENSOR DIFFERENCE: %d!\n", t - b);
                    control->stop();
                    qualityAssurance--;
                    if (qualityAssurance <= 0) {
                        bumperTimer = 2;
                        currentTask = PushTheButton;
                    }
                    break;
                }
                else {
                    printf("WALL\n");
                    control->turnSingle(turnDirection*30);
                }
            }
            qualityAssurance = 3;
            break;
        }
        
        case MoveToCentre: {
            double modifier = (sensing->getRightWhisker() || sensing->getRightBumper()) ? -1 : (sensing->getLeftWhisker() || sensing->getLeftBumper()) ? 1 : turnDirection;
            printf("MOVING TO CENTRE; mod: %f\n", modifier);
            improveTimer -= delta;
            if (improveTimer > 0) {
                control->curvedBacking(modifier * 30);
            } else {
                improveTimer = 0.7;
                currentTask = improveDistance;
            }
            break;
        }
        
        case PushTheButton: {
            printf("PUSH THE BUTTON\n");
            
            if (sensing->getLeftBumper() || sensing->getRightBumper()) { 
                printf("BUMPER\n");
                control->stop();
                control->moveSlow(30);
                if (bumperTimer > 0) {
                    bumperTimer -= delta;
                }
                
                if (bumperTimer <= 0) {
                    improveTimer = 0.5;
                    turnDirection = sensing->getLeftBumper() ? -1 : 1;
                    currentTask = improveAngle;
                } else {
                    if ((sensing->getLeftLight() > 300) || (sensing->getRightLight() > 300)) {
                        bumperTimer = 1;
                    }
                }
            } else {
                printf("FWD\n");
                control->move(30);
            }
            break;
        }        
        
        case improveDistance: {
            printf("IMPROVING DISTANCE\n");
            
            improveTimer -= delta;
            if (improveTimer > 0) {
                control->moveSlow(-10);
            }  else {
                //improveTimer = 0.5;
                control->stop();
                turnDirection *= -1;
                currentTask = Align;
            }
            break;
        }
        
        case improveAngle: {
            printf("IMPROVING ANGLE\n");
            
            improveTimer -= delta;
            if (improveTimer > 0) {
                control->turnSingle(turnDirection * 30);
                break;
            }
            improveTimer = 0.7;
            currentTask = improveDistance;
            break;
        }
    }
}

void HitButtonStrategy::reset() {
    printf("RESET\n");
    directionProtection = 0;
    control->stop();
    currentTask = Align;
    qualityAssurance = 3;
    darkAreaCooldown = 2;
    servo->fix();
    cameFromLeft = sensing->isLeftOnBlack() && !sensing->isRightOnBlack();
    if (cameFromLeft) {
        printf("LEFT!!\n");
        turnDirection = 1;
    } else {
        printf("RIGHT!!\n");
        turnDirection = -1;
    }
}

