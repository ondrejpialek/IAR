#ifndef SENSING_H_
#define SENSING_H_

#include <time.h>

#include "interfaceKit.h"
#include "averagedArray.h"
#include "util.h"

class Sensing : public InterfaceKitCallbackHandler {
    private:        
        AveragedArray<int>* sensorReadings[8];
        AveragedArray<int>* inputReadings[8];
        timespec cache;
        double cachedFrequency;
        int grayFloorLevel[2];
        
        int getDistance(int sensor);
        int getSonarDistance(int sensor);
        bool getInput(int sensor);
        bool isOnBlack(int sensor);
        double getFrequency(int sensor);
	int histogram [180];
        
    public:
        Sensing();
        virtual ~Sensing();
        
        virtual void OnSensorChange(int index, int value);
        virtual void OnInputChange(int index, int value);
        
        void adjustFloorLevel();
        
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
        int getSonarDistance();
        double getFrequency();
	
	int getHistogram();
};

#endif /* SENSING_H_ */
