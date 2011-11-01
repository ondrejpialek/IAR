#include <cmath>

#define PI 3.14159265
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#include "strategy.h"

Strategy::Strategy(Sensing* sensing, Control* control, ServoControl* servo) {
    this->sensing = sensing;
    this->control = control;
    this->servo = servo;
}

double FindSiteStrategy::getUtility() {
    double utility = 0.8;
    if (sensing->isLeftOnBlack() || sensing->isRightOnBlack()) {
        utility = 0.1;
    }
    return utility;
}

void FindSiteStrategy::reset() {
    servo->doScan();
    sonarCooldown = 4.8;
    currentTask = Scan;
    
}

void FindSiteStrategy::step(double delta, bool firstRun) {
    if (firstRun) { reset(); }
    
    int sonarLeft = sensing->getSonarDistance(-1);
    int sonarFront = sensing->getSonarDistance(0);
    int sonarRight = sensing->getSonarDistance(1);
    
    if (sensing->getLeftBumper() || sensing->getRightBumper()) {
        moveTimer = 0.8;
        currentTask = Main;
    } else if ((sonarFront < 20) || (distanceFix > 0)) {
        if ((sonarFront < 20) && (distanceFix <= 0)) {
            distanceFix = 0.3;
        }
        
        distanceFix -= delta;
        
        int modifier;
        if (sonarLeft > sonarRight) {
            modifier = -1;
        } else {
            modifier = 1;
        }

        control->curvedFronting(modifier*45);
        return;
    }
     
    switch (currentTask) {
        case Scan: {
            if (sonarCooldown >= 0) {
                control->stop();
                sonarCooldown -= delta;
            }  else {
                int shortestDistance = MIN(sonarFront, MIN(sonarLeft, sonarRight));
                printf("shortest %d\n", shortestDistance);
                if (shortestDistance < 130) {
                    printf("Within range\n");
                    if ((shortestDistance == sonarFront) || (shortestDistance < 30)) {
                        goTo = 0;
                        moveTimer = 0.8;
                        currentTask = Main;
                        break;
                    } else if (shortestDistance == sonarLeft) { 
                        goTo = -1;
                    } else if (shortestDistance == sonarRight) {
                        goTo = 1;
                    }
                }
                else { 
                    goTo = 1; 
                }
                
                turnTimer = 0.1;
                currentTask = Turn;
            }
            break;
        }
        
        case Turn: {
            if (turnTimer > 0) {
                if ((sensing->getLeftWhisker()) || (sensing->getRightWhisker())) {
                    control->move(30);
                } else {
                    turnTimer -= delta;
                    control->turn(goTo * 30);
                }
            } else {
                sonarCooldown = 1; 
                currentTask = Scan; 
            }
            break;
        }
        
        case Reallign: {
            
        }
        
        case Main: {
            
            #ifndef SILENT_STRATEGY
            printf("L:%8d R:%8d\n", left, right);
            #endif
            if (moveTimer > 0) {
                moveTimer -= delta;
                
                if (sensing->getLeftBumper() || sensing->getRightBumper()) {
                    
                    control->move(-20);
                    wasBumper = 0.45;
                    
                    #ifndef SILENT_STRATEGY
                    printf("BUM: back!\n");
                    #endif
                } else if (wasBumper > 0) {
                    int modifier;
                    if (sonarLeft > sonarRight) {
                        modifier = -1;
                    } else {
                        modifier = 1;
                    }
                    
                    wasBumper -= delta;
                    control->turnSlow(modifier*45);
                } else {
                    control->move(20);
                }
            }
            else {
                sonarCooldown = 4.8;
                currentTask = Scan;
            }

            break;
            }
            
        }
    }
    
    double HitButtonStrategy::getUtility() {
        double utility = 0;
        
        if ((darkAreaCooldown > 0) || sensing->isLeftOnBlack() || sensing->isRightOnBlack()) {
            utility = 0.9;
        }
        if (ttl < 0) {
            ttl += 0.02;
            return 0;
        }
        return utility;
    }
    
    void HitButtonStrategy::step(double delta, bool firstRun) {    
        if (firstRun)
            reset();
        
        ttl -= delta;
        darkAreaCooldown -= delta;
        
        if (sensing->isLeftOnBlack() || sensing->isRightOnBlack())
            darkAreaCooldown = 3.5;
        
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
                    
                    if (((b == 0) || (b > 80)) && (sensing->getSonarDistance(0) > 45)) {
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
                    else if ((((t > b+5) || (t == 0)) && (directionProtection <= 0)) && (sensing->getSonarDistance(0) < 65)) {
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
        ttl = 15.0;
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
    
    