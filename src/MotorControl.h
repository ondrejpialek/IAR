#include <phidget21.h>

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_

	void initMotors();
	void closeMotors();

	void setVelocity(int motor, double velocity);
	void setAcceleration(int motor, double velocity);
	double getMaxVelocity(int motor);
	double getMaxAcceleration(int motor);


#endif /* MOTORCONTROL_H_ */
