#ifndef SERVOCONTROL_H_
#define SERVOCONTROL_H_

#include <phidget21.h>

class ServoControl {
    private:
        CPhidgetAdvancedServoControlHandle servo;
        bool initialized;
        void ensureInitialized();
	double currentPosition;
        
    public: 
        ServoControl();
        virtual ~ServoControl();
	virtual void PositionChange(int index, int value) = 0;
        
        void setVelocity(int motor, double velocity);
        void setAcceleration(int motor, double velocity);
	void setPosition(int motor, double newPosition);
	int getPosition(int motor, double* currentPosition);
};




#endif /* MOTORCONTROL_H_ */
