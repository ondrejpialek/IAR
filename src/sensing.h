#ifndef SENSING_H_
#define SENSING_H_

#include "interfaceKit.h"
#include "averagedArray.h"

class Sensing : public InterfaceKitCallbackHandler {
    private:        
        AveragedArray<int>* sensorReadings[8];
        AveragedArray<int>* inputReadings[8];
        int grayFloorLevel[2];
        
        int getDistance(int sensor);
        int getSonarDistance(int sensor);
        bool getInput(int sensor);
        bool isOnBlack(int sensor);
        
    public:
        Sensing();
        virtual ~Sensing();
        
        virtual void OnSensorChange(int index, int value);
        virtual void OnInputChange(int index, int value);
        
        void adjustFloorLevel();
        
        int getLeftDistance();
        int getRightDistance();
        bool getLeftWhisker();
        bool getRightWhisker();
        bool getLeftBumper();
        bool getRightBumper();
        bool isLeftOnBlack();
        bool isRightOnBlack();
        int getLeftLight();
        int getRightLight();
        int getSonarDistance();
};

#endif /* SENSING_H_ */
