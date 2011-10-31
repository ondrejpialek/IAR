#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "control.h"
#include "sensing.h"
#include "servoControl.h"

class Strategy {
    protected:
        Sensing* sensing;
        Control* control;
	ServoControl* servo;
        
    public:
        virtual void step(double delta, bool firstRun) = 0;
        virtual double getUtility() = 0;
        
        Strategy(Sensing* sensing, Control* control, ServoControl* servo);
};

enum FindSiteStrategyTask { Scan, Turn, Main };

class FindSiteStrategy : public Strategy {
    private:
        double wasBumper;
	double sonarCooldown;
	double moveTimer;
	double turnTimer;
	int goTo;
	FindSiteStrategyTask currentTask;
	
	void reset();
        
    public:
        virtual void step(double delta, bool firstRun);
        virtual double getUtility();
        
        FindSiteStrategy(Sensing* sensing, Control* control, ServoControl* servo) : Strategy(sensing, control, servo) { };
};

enum HitButtonStrategyTask { Align, PushTheButton, improveDistance, improveAngle, MoveToCentre };

class HitButtonStrategy : public Strategy {
    private:
        HitButtonStrategyTask currentTask;
        bool cameFromLeft;
        double bumperTimer;
        double improveTimer;
        double turningToButton;
        double turningProtection;
        int turnDirection;
        int maxDistanceWhileTurning;
        double victoryBacking;
        double dancing;
        double directionProtection;
        int qualityAssurance;
        double darkAreaCooldown;
        void reset();
        
    public:
        virtual void step(double delta, bool firstRun);
        virtual double getUtility();
        
        HitButtonStrategy(Sensing* sensing, Control* control, ServoControl* servo) : Strategy(sensing, control, servo) { };
};

#endif /* STRATEGY_H_ */