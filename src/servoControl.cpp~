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

int AttachHandler(CPhidgetHandle ADVSERVO, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (ADVSERVO, &name);
	CPhidget_getSerialNumber(ADVSERVO, &serialNo);
	printf("%s %10d attached!\n", name, serialNo);

	return 0;
}

int DetachHandler(CPhidgetHandle ADVSERVO, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (ADVSERVO, &name);
	CPhidget_getSerialNumber(ADVSERVO, &serialNo);
	printf("%s %10d detached!\n", name, serialNo);

	return 0;
}

int ErrorHandler(CPhidgetHandle ADVSERVO, void *userptr, int ErrorCode, const char *Description)
{
	printf("Error handled. %d - %s\n", ErrorCode, Description);
	return 0;
}

int PositionChangeHandler(CPhidgetAdvancedServoHandle ADVSERVO, void *usrptr, int Index, double Value)
{
	printf("Motor: %d > Current Position: %f\n", Index, Value);
	return 0;
}

//Display the properties of the attached phidget to the screen.  We will be displaying the name, serial number and version of the attached device.
int display_properties(CPhidgetAdvancedServoHandle phid)
{
	int serialNo, version, numMotors;
	const char* ptr;

	CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
	CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
	CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);

	CPhidgetAdvancedServo_getMotorCount (phid, &numMotors);

	printf("%s\n", ptr);
	printf("Serial Number: %10d\nVersion: %8d\n# Motors: %d\n", serialNo, version, numMotors);

	return 0;
}

void MotorControl::ensureInitialized() {
    if (!initialized) {
        int result;
        const char *err;
	
	//Declare an advanced servo handle
	CPhidgetAdvancedServoHandle servo = 0;

	//create the advanced servo object
	CPhidgetAdvancedServo_create(&servo);

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)servo, AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)servo, DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)servo, ErrorHandler, NULL);

	//Registers a callback that will run when the motor position is changed.
	//Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetAdvancedServo_set_OnPositionChange_Handler(servo, PositionChangeHandler, NULL);

	//open the device for connections
	CPhidget_open((CPhidgetHandle)servo, -1);
        
        //get the program to wait for a motor control device to be attached
        printf("Waiting for MotorControl to be attached....");
        if ((result = CPhidget_waitForAttachment((CPhidgetHandle) servo, 10000))) {
            CPhidget_getErrorDescription(result, &err);
            printf("Problem waiting for attachment: %s\n", err);
            throw - 1;
        }
        
        //Set up some initial acceleration and velocity values
	CPhidgetAdvancedServo_getAccelerationMin(servo, 0, &minAccel);
	CPhidgetAdvancedServo_setAcceleration(servo, 0, minAccel*2);
	CPhidgetAdvancedServo_getVelocityMax(servo, 0, &maxVel);
	CPhidgetAdvancedServo_setVelocityLimit(servo, 0, maxVel/2);
        
        initialized = true;
    }
}

ServoControl::ServoControl() { }

ServoControl::~ServoControl() {
    CPhidget_close((CPhidgetHandle) servo);
    CPhidget_delete((CPhidgetHandle) servo);  
}

void ServoControl::setVelocity(double velocity) {
    ensureInitialized();
    CPhidgetAdvancedServo_setVelocity((CPhidgetAdvancedServoHandle) servo, 0, velocity);
}

void ServoControl::setAcceleration(double acceleration) {
    ensureInitialized();
    CPhidgetAdvancedServo_setAcceleration((CPhidgetAdvancedServoHandle) servo, 0, acceleration);
}

void ServoControl::setPosition(double position) {
    ensureInitialized();
    CPhidgetAdvancedServo_setPosition((CPhidgetAdvancedServoHandle) servo, 0, position);
}

void ServoControl::getPosition(double position) {
    ensureInitialized();
    CPhidgetAdvancedServo_getPosition((CPhidgetAdvancedServoHandle) servo, 0, &currentPosition);
}

int PositionChangeHandler(CPhidgetAdvancedServoHandle servo, void *usrptr, int i, int v)
{
    #ifndef SILENT 
    printf("Input: %d > Value: %d\n", i, v);
    #endif
    
    AdvancedServo* handler = (AdvancedServo*)usrptr;
    sensing->buildHistogram(round(v/125));
        
    return 0;
}