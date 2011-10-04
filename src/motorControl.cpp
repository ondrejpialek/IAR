#include "motorControl.h"
#include <stdio.h>
#include <phidget21.h>
#include "phidget.h"

CPhidgetMotorControlHandle handle;

void closeMotors()
{
    CPhidget_close((CPhidgetHandle) handle);
    CPhidget_delete((CPhidgetHandle) handle);
}


int InputChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, int State)
{
    #ifndef SILENT
    printf("Input %d > State: %d\n", Index, State);
    #endif
    return 0;
}

int VelocityChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value)
{
    #ifndef SILENT
    printf("Motor %d > Current Speed: %f\n", Index, Value);
    #endif    
    return 0;
}

int CurrentChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value)
{
    #ifndef SILENT
    printf("Motor: %d > Current Draw: %f\n", Index, Value);
    #endif
    return 0;
}


void initMotors()
{
    int result;
    const char *err;

    //create the motor control object
    CPhidgetMotorControl_create(&handle);

    //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
    CPhidget_set_OnAttach_Handler((CPhidgetHandle) handle, AttachHandler, NULL);
    CPhidget_set_OnDetach_Handler((CPhidgetHandle) handle, DetachHandler, NULL);
    CPhidget_set_OnError_Handler((CPhidgetHandle) handle, ErrorHandler, NULL);

    //Registers a callback that will run if an input changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnInputChange_Handler((CPhidgetMotorControlHandle) handle, InputChangeHandler, NULL);

    //Registers a callback that will run if a motor changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnVelocityChange_Handler((CPhidgetMotorControlHandle) handle, VelocityChangeHandler, NULL);

    //Registers a callback that will run if the current draw changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnCurrentChange_Handler((CPhidgetMotorControlHandle) handle, CurrentChangeHandler, NULL);

    //open the motor control for device connections
    CPhidget_open((CPhidgetHandle) handle, -1);

    //get the program to wait for a motor control device to be attached
    printf("Waiting for MotorControl to be attached....");
    if ((result = CPhidget_waitForAttachment((CPhidgetHandle) handle, 10000))) {
	CPhidget_getErrorDescription(result, &err);
	printf("Problem waiting for attachment: %s\n", err);
	throw - 1;
    }
}

void setVelocity(int motor, double velocity)
{
    CPhidgetMotorControl_setVelocity((CPhidgetMotorControlHandle) handle, motor, velocity);
}

void setAcceleration(int motor, double acceleration)
{
    CPhidgetMotorControl_setAcceleration((CPhidgetMotorControlHandle) handle, motor, acceleration);
}

double getMaxAcceleration(int motor)
{
    double val;
    CPhidgetMotorControl_getAccelerationMax((CPhidgetMotorControlHandle) handle, motor, &val);
    return val;
}