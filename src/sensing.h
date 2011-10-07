#ifndef SENSING_H_
#define SENSING_H_

#include "interfaceKit.h"

#define NANOSECONDS_PER_SECOND 1E9

class Sensing : public InterfaceKitCallbackHandler {
  int sensorReadings[8];
  timespec inputTime[8];
  int inputReadings[8];
  
  private:
    int getDistance(int sensor);
    bool getWhisker(int sensor);
  
  public:
    Sensing();
    virtual ~Sensing();
    
    virtual void OnSensorChange(int index, int value);
    virtual void OnInputChange(int index, int value);
    
    int getLeftDistance();
    int getRightDistance();
    bool getFrontWhisker();
    bool getBackWhisker();
};

#endif /* SENSING_H_ */
