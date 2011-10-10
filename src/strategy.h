#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "control.h"
#include "sensing.h"

class Strategy {
    protected:
        Sensing* sensing;
        Control* control;
    
    public:
        virtual void step(double delta) = 0;
        virtual double getUtility() = 0;
        
        Strategy(Sensing* sensing, Control* control);
};

class FindSiteStrategy : public Strategy {
    private:
        double wasWhisker;
    
    public:
        virtual void step(double delta);
        virtual double getUtility();
        
        FindSiteStrategy(Sensing* sensing, Control* control) : Strategy(sensing, control) {
        
        };
};

class HitButtonStrategy : public Strategy {
    public:
        virtual void step(double delta);
        virtual double getUtility();
        
        HitButtonStrategy(Sensing* sensing, Control* control) : Strategy(sensing, control) { };
};

#endif /* STRATEGY_H_ */