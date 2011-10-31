#ifndef SENSING_H_
#define SENSING_H_

#include <time.h>

#include "interfaceKit.h"
#include "averagedArray.h"
#include "util.h"
#include "servoControl.h"

#define SONAR_DIRECTIONS 3

class Sensing : public InterfaceKitCallbackHandler, public ServoEventHandler {
    private:        
        AveragedArray<int>* sensorReadings[8];
        AveragedArray<int>* inputReadings[8];
        timespec cache;
        double cachedFrequency;
        
        int getDistance(int sensor);
        bool getInput(int sensor);
        bool wasLeftOnBlack;
        bool wasRightOnBlack;
        int isOnBlack(int sensor);
        double getFrequency(int sensor);
        
        int latestSonarIndex;
        int latestSonar;
        double latestPosition;
        AveragedArray<int>* sonarReadings[SONAR_DIRECTIONS];
        int getSonarIndex(double position);
        
    public:
        Sensing();
        virtual ~Sensing();
        
        virtual void OnSensorChange(int index, int value);
        virtual void OnInputChange(int index, int value);
        
        virtual void OnPositionChanged(double position);
        
        int getTopDistance();
        int getBottomDistance();
        bool getLeftWhisker();
        bool getRightWhisker();
        bool getLeftBumper();
        bool getRightBumper();
        bool isLeftOnBlack();
        bool isRightOnBlack();
        int getLeftLight();
        int getRightLight();
        int getSonarDistance(int section);
        double getFrequency();
};

#endif /* SENSING_H_ */
