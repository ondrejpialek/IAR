#include <stdio.h>
#include <phidget21.h>

#include "motorControl.h"
#include "phidget.h"

CPhidgetMotorControlHandle handle;

void MotorControl::ensureInitialized() {
    if (!initialized) {
        int result;
        const char *err;
        
        //create the motor control object
        CPhidgetMotorControl_create(&controlHandle);
        
        //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
        CPhidget_set_OnAttach_Handler((CPhidgetHandle) controlHandle, AttachHandler, NULL);
        CPhidget_set_OnDetach_Handler((CPhidgetHandle) controlHandle, DetachHandler, NULL);
        CPhidget_set_OnError_Handler((CPhidgetHandle) controlHandle, ErrorHandler, NULL);
        
        //open the motor control for device connections
        CPhidget_open((CPhidgetHandle) controlHandle, -1);
        
        //get the program to wait for a motor control device to be attached
        printf("Waiting for MotorControl to be attached....");
        if ((result = CPhidget_waitForAttachment((CPhidgetHandle) controlHandle, 10000))) {
            CPhidget_getErrorDescription(result, &err);
            printf("Problem waiting for attachment: %s\n", err);
            throw - 1;
        } 
        initialized = true;
    }
}

MotorControl::MotorControl() { }

MotorControl::~MotorControl() {
    CPhidget_close((CPhidgetHandle) controlHandle);
    CPhidget_delete((CPhidgetHandle) controlHandle);  
}

void MotorControl::setVelocity(int motor, double velocity) {
    ensureInitialized();
    CPhidgetMotorControl_setVelocity((CPhidgetMotorControlHandle) controlHandle, motor, velocity);
}

void MotorControl::setAcceleration(int motor, double acceleration) {
    ensureInitialized();
    CPhidgetMotorControl_setAcceleration((CPhidgetMotorControlHandle) controlHandle, motor, acceleration);
}
