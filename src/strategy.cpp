#include "strategy.h"

Strategy::Strategy(Sensing* sensing, Control* control) {
    this->sensing = sensing;
    this->control = control;
}

double FindSiteStrategy::getUtility() {
    double utility = 1;
    if (sensing->isLeftOnBlack() || sensing->isRightOnBlack()) {
        utility = 0;
    }
    return utility;
}

void FindSiteStrategy::step(double delta) {
    int left = sensing->getLeftDistance();
    int right = sensing->getRightDistance();
    bool fWhisker = sensing->getFrontWhisker();
    bool bWhisker = sensing->getBackWhisker();
    
    printf("L:%8d R:%8d\n", left, right);
    
    if (bWhisker && fWhisker) {
        double mod = 1;
        if (left > right) {
            mod = -1;
        }
        control->turn(mod * 90);
        printf("WW: PANIC\n");
    } else if (bWhisker) {
        wasWhisker = 0.8;
        printf("BW: forward\n");
        control->move(20);
    } else if (fWhisker) {
        wasWhisker = 0.8;
        printf("FW: back\n");
        control->move(-20);
    } else if (wasWhisker > 0) {
        wasWhisker -= delta;
        printf("WT: turning from whisker\n");
        control->turn(45);
    } else if ((left < 6 || left > 500) && (right < 6 || right > 500)) {
        control->move(20);
        printf("CLEAR: forward\n");
    } else if (left > 6 && left < 20) {
        control->turn(-50);
        printf("LE: right\n");
    } else if (right > 6 && right < 20) {
        control->turn(50);
        printf("RG: left\n");
    } else {
        control->move(20);
        printf("EL: forward\n");
    }    
}

double HitButtonStrategy::getUtility() {
    double utility = 0;
    if (sensing->isLeftOnBlack() || sensing->isRightOnBlack()) {
        utility = 1;
    }
    return utility;
}

void HitButtonStrategy::step(double delta) {
    control->stop();
}