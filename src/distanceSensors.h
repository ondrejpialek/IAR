#ifndef DISTANCESENSORS_H_
#define DISTANCESENSORS_H_

#include <stdio.h>
#include <phidget21.h>

extern int readings [8];
void initDistanceSensors();
void closeDistanceSensors();

#endif /* DISTANCESENSORS_H_ */
