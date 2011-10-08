#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_

#include <phidget21.h>

class MotorControl {
    private:
        CPhidgetMotorControlHandle controlHandle;
        bool initialized;
        void ensureInitialized();
        
    public: 
        MotorControl();
        virtual ~MotorControl();
        
        void setVelocity(int motor, double velocity);
        void setAcceleration(int motor, double velocity);
};




#endif /* MOTORCONTROL_H_ */
