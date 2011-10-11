#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "control.h"
#include "sensing.h"

class Strategy {
    protected:
        Sensing* sensing;
        Control* control;
    
    public:
        virtual void step(double delta, bool firstRun) = 0;
        virtual double getUtility() = 0;
        
        Strategy(Sensing* sensing, Control* control);
};

class FindSiteStrategy : public Strategy {
    private:
        double wasWhisker;
    
    public:
        virtual void step(double delta, bool firstRun);
        virtual double getUtility();
        
        FindSiteStrategy(Sensing* sensing, Control* control) : Strategy(sensing, control) { };
};

enum HitButtonStrategyTask { Align, GetWhiskerContact, FindCentre, PushTheButton };

class HitButtonStrategy : public Strategy {
    private:
        HitButtonStrategyTask currentTask;
        bool cameFromLeft;
        int turnDirection;
        double directionProtection;
        double distances [256];
        int distanceCount;
        double getDeviation(int distance);
        void reset();
    
    public:
        virtual void step(double delta, bool firstRun);
        virtual double getUtility();
        
        HitButtonStrategy(Sensing* sensing, Control* control) : Strategy(sensing, control) { };
};

#endif /* STRATEGY_H_ */