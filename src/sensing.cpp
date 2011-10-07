#include <time.h>

#include "sensing.h"
#include "interfaceKit.h"

Sensing::Sensing() : InterfaceKitCallbackHandler() { }

Sensing::~Sensing() { }

void Sensing::OnSensorChange(int index, int value) {
    sensorReadings[index] = value;  
}

void Sensing::OnInputChange(int index, int value) {
    inputReadings[index] = value;
    clock_gettime(CLOCK_MONOTONIC, &inputTime[index]);
}

int Sensing::getDistance(int sensor)
{
    if (sensorReadings[sensor] <= 20)
      return 0;
    
    return 4080 / (sensorReadings[sensor] - 20);
}

int Sensing::getLeftDistance()
{
    return getDistance(1);
}

int Sensing::getRightDistance()
{
    return getDistance(2);
}

bool Sensing::getWhisker(int sensor) {
    timespec now, then;
    clock_gettime(CLOCK_MONOTONIC, &now);
    
    then = inputTime[sensor];
    
    double diff = (now.tv_sec - then.tv_sec) + ((now.tv_nsec - then.tv_nsec) / NANOSECONDS_PER_SECOND);
    
    if (diff < 0.2) {
	return false;
    } else {
	return inputReadings[sensor] == 1;
    }
}

bool Sensing::getFrontWhisker()
{
    return getWhisker(1);
}

bool Sensing::getBackWhisker()
{
    return getWhisker(0);
}
