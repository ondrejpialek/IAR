// - Advanced Servo simple -
// This simple example sets up an Advanced Servo object, hooks the event handlers and opens it for device connections.  
// Once an Advanced Servo is attached it will move the motor to various positions, displaying the event details to the console.
//
// Copyright 2008 Phidgets Inc.  All rights reserved.
// This work is licensed under the Creative Commons Attribution 2.5 Canada License. 
// view a copy of this license, visit http://creativecommons.org/licenses/by/2.5/ca/

#include <stdio.h>
#include <phidget21.h>
#include <cmath>
#include "servoControl.h"
#include "phidget.h"

int PositionChangeHandler(CPhidgetAdvancedServoHandle ADVSERVO, void *usrptr, int i, double v)
{
	//printf("Motor: %d > Current Position: %f\n", i, v);
	ServoControl* handler = (ServoControl*)usrptr;
	handler->OnPositionChange(i, v);
	
	return 0;
}

void ServoControl::OnPositionChange(int i, double v) {
    position = v;
    
    if (handler != NULL) {
        handler->OnPositionChanged(position);
    }
    
    if(moving) {
        if (v < SONAR_MIN + 1) {
            setPosition(SONAR_MAX);
        } else if (v > SONAR_MAX - 1) {
            setPosition(SONAR_MIN);
        }
    }
}

void ServoControl::doScan() {
    if (!moving) {
        moving = true;  
        
        double position = getPosition();
        
        if (position < 1) {
            setPosition(SONAR_MAX);
        } else {
            setPosition(SONAR_MIN);
        }
    }
}

void ServoControl::fix() {
    moving = false;
    setPosition((SONAR_MAX - SONAR_MIN) / 2);
}

void ServoControl::ensureInitialized() {
    if (!initialized) {
        int result;
        const char *err;

	printf("initializing!");
	
	//create the advanced servo object
	CPhidgetAdvancedServo_create(&servo);

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)servo, AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)servo, DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)servo, ErrorHandler, NULL);

	//Registers a callback that will run when the motor position is changed.
	//Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetAdvancedServo_set_OnPositionChange_Handler(servo, PositionChangeHandler, this);

	//open the device for connections
	CPhidget_open((CPhidgetHandle)servo, -1);
        
        //get the program to wait for a motor control device to be attached
        //printf("Waiting for MotorControl to be attached....");
        if ((result = CPhidget_waitForAttachment((CPhidgetHandle) servo, 10000))) {
            CPhidget_getErrorDescription(result, &err);
            //printf("Problem waiting for attachment: %s\n", err);
            throw - 1;
        }
       // double maxVel;
        //Set up some initial acceleration and velocity values
        //	CPhidgetAdvancedServo_getAccelerationMin(servo, 0, &minAccel);
        //	CPhidgetAdvancedServo_setAcceleration(servo, 0, minAccel*2);
        //CPhidgetAdvancedServo_getVelocityMax(servo, 0, &maxVel);
        //CPhidgetAdvancedServo_setVelocityLimit(servo, 0, maxVel/4);
        
        initialized = true;
        setPosition(0.0);
        CPhidgetAdvancedServo_setEngaged((CPhidgetAdvancedServoHandle) servo, 0, 1);
    }
}

ServoControl::ServoControl() {
    handler = NULL;
}

void ServoControl::setEventHandler(ServoEventHandler* handler) {
    this->handler = handler;
    if (handler != NULL) {
        handler->OnPositionChanged(readPosition());
    }
}

ServoControl::~ServoControl() {
    CPhidgetAdvancedServo_setEngaged((CPhidgetAdvancedServoHandle) servo, 0, 0);
    CPhidget_close((CPhidgetHandle) servo);
    CPhidget_delete((CPhidgetHandle) servo);  
}

void ServoControl::setVelocity(double velocity) {
    ensureInitialized();
    CPhidgetAdvancedServo_setVelocityLimit((CPhidgetAdvancedServoHandle) servo, 0, velocity);
}

void ServoControl::setAcceleration(double acceleration) {
    ensureInitialized();
    CPhidgetAdvancedServo_setAcceleration((CPhidgetAdvancedServoHandle) servo, 0, acceleration);
}

void ServoControl::setPosition(double position) {
    ensureInitialized();
    
    CPhidgetAdvancedServo_setPosition((CPhidgetAdvancedServoHandle) servo, 0, position);   
}

double ServoControl::readPosition() {
    CPhidgetAdvancedServo_getPosition((CPhidgetAdvancedServoHandle) servo, 0, &position);    
    return position;
}

double ServoControl::getPosition() {
    ensureInitialized();    
    return position;
}