#ifndef CONTROL_H_
#define CONTROL_H_

#include "motorControl.h"
#include "servoControl.h"

extern double LEFT_MOTOR_MODIFIER;
extern double RIGHT_MOTOR_MODIFIER;

class Control {
    private:
        double remaining;
        bool initialized;
        void ensureInitialized();
        MotorControl* control;
	ServoControl* servo;
        void turn(double degrees, double velocity);
        
    public:
        Control();
        ~Control();
        
        void move(double distance);
        void curvedBacking(double distance);
        void moveSlow(double distance);
        void turn(double degrees);
        void turnSlow(double degrees);
        void turnSingle(double degrees);
        void stop();
        void controlTick(double diff);    
        void curvedFronting(double distance);
};

#endif /* CONTROL_H_ */
