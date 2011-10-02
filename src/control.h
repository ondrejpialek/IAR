#ifndef CONTROL_H_
#define CONTROL_H_

#include "motorControl.h"

extern double LEFT_MOTOR_MODIFIER;
extern double RIGHT_MOTOR_MODIFIER;

void initControl();
void move(double distance);
void turn(double degrees);
void stop();
void controlTick(double diff);
void releaseControl();

#endif /* CONTROL_H_ */
