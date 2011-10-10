#include <time.h>

#include "sensing.h"
#include "interfaceKit.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))


Sensing::Sensing() : InterfaceKitCallbackHandler() {
    grayFloorLevel = 20;
}

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

bool Sensing::isOnBlack(int sensor) {
    return (sensorReadings[sensor] <= grayFloorLevel - 3); 
}

void Sensing::adjustFloorLevel() {
    ensureInitialized();
    grayFloorLevel = MIN(sensorReadings[3], sensorReadings[4]);
    printf("FLOOR: %d", grayFloorLevel);
}

int Sensing::getLeftDistance()
{
    ensureInitialized();
    return getDistance(1);
}

int Sensing::getRightDistance()
{
    ensureInitialized();
    return getDistance(2);
}

bool Sensing::getFrontWhisker()
{
    ensureInitialized();
    return getWhisker(1);
}

bool Sensing::getBackWhisker()
{
    ensureInitialized();
    return getWhisker(0);
}

bool Sensing::isLeftOnBlack() {
    ensureInitialized();
    return isOnBlack(3);    
}

bool Sensing::isRightOnBlack() {
    ensureInitialized();
    return isOnBlack(4);    
}
