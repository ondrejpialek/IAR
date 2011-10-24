#include <time.h>
#include <math.h>

#include "sensing.h"
#include "interfaceKit.h"
#include "util.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))


Sensing::Sensing() : InterfaceKitCallbackHandler() {
    grayFloorLevel = { 20, 20 };
    cachedFrequency = -1;
    for (int i = 0; i < 8; i++) {
        inputReadings[i] = new AveragedArray<int>(0.1);
        switch (i+1) {
            case RightFrontLight:
            case LeftFrontLight:
                sensorReadings[i] = new AveragedArray<int>(0.05);
                break;
            case TopIR:
                sensorReadings[i] = new AveragedArray<int>(0.07);
                break;
            case BottomIR:
                sensorReadings[i] = new AveragedArray<int>(0.1);
                break;
            default:
                sensorReadings[i] = new AveragedArray<int>(0.5);
        }
    }
    //histogram = new int [180];
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

bool Sensing::getInput(int sensor) {
    return inputReadings[sensor]->getLatest();
}

bool Sensing::isOnBlack(int sensor) {
    int gray = grayFloorLevel[sensor - RightBottomLight];
    bool black = (sensorReadings[sensor]->getLatest() < gray - 10) || (sensorReadings[sensor]->getLatest() >= gray + 20);
    /*if (!black) {
       printf("BLACK LOST (%d) (l, c, u): (%d, %d, %d)\n", sensor, gray - 10, sensorReadings[sensor]->getLatest(), gray + 20);    
    }*/
    return black;
}

void Sensing::adjustFloorLevel() {
    ensureInitialized();
    grayFloorLevel[0] = sensorReadings[RightBottomLight]->getLatest();
    grayFloorLevel[1] = sensorReadings[LeftBottomLight]->getLatest();
    printf("FLOOR L: %d, R:%d\n", grayFloorLevel[0], grayFloorLevel[1]);
}

int Sensing::getTopDistance()
{
    ensureInitialized();
    return getDistance(TopIR);
}

int Sensing::getBottomDistance()
{
    ensureInitialized();
    return getDistance(BottomIR);
}

int Sensing::getSonarDistance()
{
    ensureInitialized();
    return getSonarDistance(7);
}

bool Sensing::getLeftWhisker()
{
    ensureInitialized();
    return getInput(1);
}

bool Sensing::getRightWhisker()
{
    ensureInitialized();
    return getInput(4);
}

bool Sensing::getLeftBumper()
{
    ensureInitialized();
    return getInput(2);
}

bool Sensing::getRightBumper()
{
    ensureInitialized();
    return getInput(3);
}

bool Sensing::isLeftOnBlack() {
    ensureInitialized();
    return isOnBlack(LeftBottomLight);    
}

bool Sensing::isRightOnBlack() {
    ensureInitialized();
    return isOnBlack(RightBottomLight);    
}

int Sensing::getRightLight() {
    ensureInitialized();
    return sensorReadings[RightFrontLight]->getLatest();
}

int Sensing::getLeftLight() {
    ensureInitialized();
    return sensorReadings[LeftFrontLight]->getLatest();
}

double Sensing::getFrequency(int sensor) {
    //printf("SENSOR: %d\n", sensor);
    
    int* values = NULL;
    timespec* times = NULL;
    int length = sensorReadings[sensor]->getLatest(6, &values, &times);
    //printf("L: %d\n", length);
    
    if (length == 0)
        return 0;
    
    //find min and max
    int min = values[0], max = values[0];
    for (int i = 0; i < length; i++) {
        if (values[i] < min)
            min = values[i];
        if (values[i] > max)
            max = values[i];        
    }
    //printf("MIN: %d, MAX: %d\n", min, max);

    //identify peaks
    int* highs = new int [length]; //0 - low, 1 - high
    int peaks = 0;
    int threshold = 0.7 * max;
    //printf("TRH: %d\n", threshold);
    
    double smooth = 1.0 / 8.0 / 2.0; //smooth up to 8Hz half of a peak
    //printf("SMTH: %f\n", smooth);
    for (int i = 0; i < length; i++) {
        int value = values[i];
        int j = i;
        while (++j < length && difference(times[i], times[j]) < smooth) {
            //double d = difference(times[i], times[j]);
            //printf("d: %f\n", d);
            value += values[j];
        }
        value /= (j-i);
        //printf("VAL[%d]: %d (%d)\n", i, value, j-i);
        
        highs[i] = (value > threshold);
        if (i > 0 && highs[i] == 1 && highs[i-1] == 0) {
            highs[i] = 2;
            peaks++;
        }
    }
    
    //printf("PEAKS: %d\n", peaks);
    
    double frequency = 0;
    
    if (peaks > 2) {
        //find delays between peaks and average them
        double* delays = new double[peaks-1];
        int prev = -1, curr = -1, peak = 0;
        double averageDelay = 0;
        for (int i = 0; i < length; i++) {
            if (highs[i] == 2) {
                curr = i;
                if (prev > -1) {
                    delays[peak] = difference(times[prev], times[curr]);
                    averageDelay += delays[peak];
                    //printf("P[%d] delay: %f\n", peak, delays[peak]);
                    peak++;
                }
                prev = curr;
            }
        }
        averageDelay /= (peaks -1);
        
        //printf("AVG DELAY: %f\n", averageDelay);
        
        double avgSqrDelay = 0;
        for (int i = 0; i < peaks-1; i++) {
            double diff = delays[i] - averageDelay;
            delays[i] = diff * diff;
            avgSqrDelay += delays[i];
        }    
        
        //printf("ERROR: %f\n", avgSqrDelay);
        
        double standardDeviation = sqrt(avgSqrDelay / (peaks - 1));
        //printf("ST DEV: %f\n", standardDeviation);
        if (standardDeviation <= 0.1) {
            frequency = round(10 / averageDelay) / 10;
        }
        
        delete [] delays;
    }
    
    delete [] values;
    delete [] times;
    delete [] highs;
    
    //printf("F: %f\n", frequency);
    
    return frequency;    
}

void Sensing::buildHistogram(double position) {
    int distance = getSonarDistance();
    
    printf("Position %f", position);
    
    histogram[(int)position] = distance;
}
/*
int getHistogram() {
    return histogram;
}*/

double Sensing::getFrequency() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    if ((cachedFrequency > -1) && (difference(cache, now) < 0.2)) {
        //printf("CACHED!\n");
        return cachedFrequency;
    }
    
    double fr = getFrequency(RightFrontLight);
    double fl = getFrequency(LeftFrontLight);
    double frequency;
    
    if (fr == 0) {
        frequency = fl;
    } else {
        frequency = fr;
    }
    
    cachedFrequency = frequency;
    clock_gettime(CLOCK_MONOTONIC, &cache);
    
    return frequency;
}
