#ifndef CONTROL_H_
#define CONTROL_H_

#include "MotorControl.h"

void initControl();
void forward(int distance);
void turn(int degrees);
void stop();
void releaseControl();

#endif /* CONTROL_H_ */
