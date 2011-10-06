#ifndef INTERFACEKIT_H_
#define INTERFACEKIT_H_

#include <stdio.h>
#include <phidget21.h>
#include <time.h>

class InterfaceKitCallbackHandler {
  public:
    virtual void SensorChangeHandler(int index, int value) = 0;
    virtual void InputChangeHandler(int index, int value) = 0;
}

extern int sensorReadings [8];
extern int inputReadings [8];
extern timespec inputTime[8];

void initInterfaceKit(InterfaceKitCallbackHandler* handler);
void closeInterfaceKit();

#endif /* INTERFACEKIT_H_ */
