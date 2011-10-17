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
    int left = sensing->getTopDistance();
    int right = sensing->getBottomDistance();

    
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
	  
	  if (sensing->getLeftBumper() || sensing->getRightBumper()) {
	    printf("HIT BUMPERS!");
	    control->moveSlow(-20);
	  }
	  else if (sensing->getRightWhisker() || sensing->getLeftWhisker()) {
	    printf("HIT WHISKERS!");
	    control->moveSlow(20);
	  }
	  else {
	    
	    int t = sensing->getTopDistance();
	    int b = sensing->getBottomDistance();
	    
	    if ((b == 0) || (b >= t)) {
	      printf("BOTTOM SENSOR = 0");
	      turnDirection = -turnDirection;
	      control->turnSlow(turnDirection*30);
	    }
	    else if ((t > b+40) && (t < b+90)) {
	      printf("SENSOR DIFFERENCE!");
	      control->stop();
	      currentTask = PushTheButton;
	    }
	    else if (t == 0) {
	      printf("TOP SENSOR = 0");
	      control->stop();
	      currentTask = PushTheButton;
	    }
	    else {
	      control->turnSlow(turnDirection*30);
	    }
	    break;
	  }
	  break;
	}
	
        case PushTheButton: {
	  timer -= delta;
	    printf("PUSH THE BUTTON");
	    
	    if (sensing->getLeftBumper() || sensing->getRightBumper()) {
		if (!bumperReaction) {
		timer = 1.0;
		bumperReaction=true;
		control->stop();
		break;
		}
		if (timer>0) {
		  if ((sensing->getLeftLight() > 400) || (sensing->getRightLight() > 400)) {
		    timer = 1.0;
		    break;
		  }
		}
		else {
		  currentTask = improveAngle;
		  break;
		}
	    }
	    control->moveSlow(30);
	    break;
        }        
        
	case improveDistance: {
	  printf("IMPROVING DISTANCE");
	  
	  improveTimer -= delta;
	  if (improveTimer > 0) {
	    control->moveSlow(-10);
	    break;
	  }
	  improveTimer = 0.5;
	  currentTask = improveAngle;
	  break;
	}
        
	case improveAngle: {
	  printf("IMPROVING ANGLE");
	  
	  improveTimer -= delta;
	  if (improveTimer > 0) {
	    control->turnSlow(turnDirection*30);
	    break;
	  }
	  currentTask = PushTheButton;
	  break;
	}
    }
}

void HitButtonStrategy::reset() {
    printf("RESET\n");
    improveTimer = 0.5;
    control->stop();
    currentTask = Align;
    cameFromLeft = sensing->isLeftOnBlack() && !sensing->isRightOnBlack();
    bumperReaction=false;
    if (cameFromLeft) {
        printf("LEFT!!\n");
        turnDirection = 1;
    } else {
        printf("RIGHT!!\n");
        turnDirection = -1;
    }
}

