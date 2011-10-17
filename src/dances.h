#ifndef DANCES_H_
#define DANCES_H_

#include "control.h"
#include "sensing.h"

enum DanceStrategyTask { Task1, Task2, Task3 };

class HalfHZDanceStrategy : public Strategy {
    private:
        DanceStrategyTask currentTask;
        double timer;
        bool started;

        void reset();
    
    public:
        virtual void step(double delta, bool firstRun);
        virtual double getUtility();
        
        HalfHZDanceStrategy(Sensing* sensing, Control* control) : Strategy(sensing, control) { };
};

class OneHZDanceStrategy : public Strategy {
    private:
        DanceStrategyTask currentTask;
        double timer;
        bool started;

        void reset();
    
    public:
        virtual void step(double delta, bool firstRun);
        virtual double getUtility();
        
        OneHZDanceStrategy(Sensing* sensing, Control* control) : Strategy(sensing, control) { };
};

class TwoHZDanceStrategy : public Strategy {
    private:
        DanceStrategyTask currentTask;
        double timer;
        bool started;
        bool stopped;
        void reset();
    
    public:
        virtual void step(double delta, bool firstRun);
        virtual double getUtility();
        
        TwoHZDanceStrategy(Sensing* sensing, Control* control) : Strategy(sensing, control) { };
};

class FourHZDanceStrategy : public Strategy {
    private:
        DanceStrategyTask currentTask;
        double timer;
        bool started;
	
        void reset();
    
    public:
        virtual void step(double delta, bool firstRun);
        virtual double getUtility();
        
        FourHZDanceStrategy(Sensing* sensing, Control* control) : Strategy(sensing, control) { };
};

class SixHZDanceStrategy : public Strategy {
    private:
        DanceStrategyTask currentTask;
        double timer;
        bool started;

        void reset();
    
    public:
        virtual void step(double delta, bool firstRun);
        virtual double getUtility();
        
        SixHZDanceStrategy(Sensing* sensing, Control* control) : Strategy(sensing, control) { };
};

class EightHZDanceStrategy : public Strategy {
    private:
        DanceStrategyTask currentTask;
        double timer;
        bool started;

        void reset();
    
    public:
        virtual void step(double delta, bool firstRun);
        virtual double getUtility();
        
        EightHZDanceStrategy(Sensing* sensing, Control* control) : Strategy(sensing, control) { };
};

#endif /* DANCES_H_ */