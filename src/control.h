#ifndef CONTROL_H_
#define CONTROL_H_

#include "motorControl.h"

extern double LEFT_MOTOR_MODIFIER;
extern double RIGHT_MOTOR_MODIFIER;

class Control {
    private:
        double remaining;
        bool initialized;
        void ensureInitialized();
        MotorControl* control;
        
    public:
        Control();
        ~Control();
        
        void move(double distance);
        void turn(double degrees);
        void stop();
        void controlTick(double diff);        
};

#endif /* CONTROL_H_ */
