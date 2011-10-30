#ifndef SERVOCONTROL_H_
#define SERVOCONTROL_H_

#include <phidget21.h>

#define SONAR_MIN 0.0
#define SONAR_MAX 180.0

class ServoEventHandler {
    public:
        virtual void OnPositionChanged(double position) = 0;
};

class ServoControl {
    private:
        CPhidgetAdvancedServoHandle servo;
        bool initialized;
        void ensureInitialized();
        bool moving;	
        double position;
        double readPosition();
        ServoEventHandler* handler;
        
    public: 
        ServoControl();
        virtual ~ServoControl();
        //virtual void PositionChange(int index, int value) = 0;
        
        void setVelocity(double velocity);
        void setAcceleration(double velocity);
        void setPosition(double newPosition);
        double getPosition();
        void OnPositionChange(int i, double v);
	
        void doScan();
        void fix();
        
        void setEventHandler(ServoEventHandler* handler);
};


#endif /* MOTORCONTROL_H_ */
