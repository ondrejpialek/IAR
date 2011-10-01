#ifndef CONTROL_H_
#define CONTROL_H_

#include "motorControl.h"

void initControl();
void move(int distance);
void turn(int degrees);
void stop();
void controlTick(double diff);
void releaseControl();

#endif /* CONTROL_H_ */
