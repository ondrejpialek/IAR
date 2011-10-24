#ifndef SERVOCONTROL_H_
#define SERVOCONTROL_H_

#include <phidget21.h>

class ServoControl {
    private:
        CPhidgetAdvancedServoHandle servo;
        bool initialized;
        void ensureInitialized();
	double currentPosition;
        
    public: 
        ServoControl();
        virtual ~ServoControl();
	//virtual void PositionChange(int index, int value) = 0;
        
        void setVelocity(double velocity);
        void setAcceleration(double velocity);
	void setPosition(double newPosition);
	double getPosition();
};




#endif /* MOTORCONTROL_H_ */
