#include <time.h>

#include "sensing.h"
#include "interfaceKit.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))


Sensing::Sensing() : InterfaceKitCallbackHandler() {
    grayFloorLevel = { 20, 20 };
    for (int i = 0; i < 8; i++) {
        inputReadings[i] = new AveragedArray<int>(0.2);
        sensorReadings[i] = new AveragedArray<int>(0.2);
    }
}

Sensing::~Sensing() {
    delete [] inputReadings;
    delete [] sensorReadings;
}

void Sensing::OnSensorChange(int index, int value) {
    sensorReadings[index]->add(value);  
}

void Sensing::OnInputChange(int index, int value) {
    inputReadings[index]->add(value);
}

int Sensing::getDistance(int sensor)
{
    if (sensorReadings[sensor]->getLatest() <= 20)
        return 0;
    
    return 4080 / (sensorReadings[sensor]->getLatest() - 20);
}

int Sensing::getSonarDistance(int sensor)
{
    return sensorReadings[sensor]->getLatest() * 1.296;
}

bool Sensing::getWhisker(int sensor) {
    return inputReadings[sensor]->getLatest();
}

bool Sensing::isOnBlack(int sensor) {
    int gray = grayFloorLevel[sensor - 3];
    bool black = (sensorReadings[sensor]->getLatest() <= gray - 15) || (sensorReadings[sensor]->getLatest() >= gray + 30);
   /* if (black) {
        printf("BLACK DETECTED F: %d, R:%d\n", gray, sensorReadings[sensor]->getLatest());    
    }*/
    return black;
}

void Sensing::adjustFloorLevel() {
    ensureInitialized();
    grayFloorLevel[0] = sensorReadings[3]->getLatest();
    grayFloorLevel[1] = sensorReadings[4]->getLatest();
    printf("FLOOR L: %d, R:%d\n", grayFloorLevel[0], grayFloorLevel[1]);
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

int Sensing::getSonarDistance()
{
    ensureInitialized();
    return getSonarDistance(6);
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
