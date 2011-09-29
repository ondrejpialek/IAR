#include <MotorControl.h>

/*
 * control.h
 *
 *  Created on: 29 Sep 2011
 *      Author: s0700409
 */

#ifndef CONTROL_H_
#define CONTROL_H_

void initControl();
void forward(int distance);
void turn(int degrees);
void stop();
void releaseControl();

#endif /* CONTROL_H_ */
