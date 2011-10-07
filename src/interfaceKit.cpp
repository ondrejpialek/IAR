#include <stdio.h>
#include <time.h>
#include <phidget21.h>
#include "interfaceKit.h"
#include "phidget.h"

int SensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int i, int v)
{
    #ifndef SILENT 
    printf("Sensor: %d > Value: %d\n", i, v);
    #endif
    
    InterfaceKitCallbackHandler* handler = (InterfaceKitCallbackHandler*)usrptr;
    handler->OnSensorChange(i, v);

    return 0;
}

int InputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int i, int v)
{
    #ifndef SILENT 
    printf("Input: %d > Value: %d\n", i, v);
    #endif
    
    InterfaceKitCallbackHandler* handler = (InterfaceKitCallbackHandler*)usrptr;
    handler->OnInputChange(i, v);
        
    return 0;
}

void InterfaceKitCallbackHandler::init() {
    int result, numSensors, i;
    const char *err;

    CPhidgetInterfaceKit_create(&ifKit);

    CPhidget_set_OnAttach_Handler((CPhidgetHandle) ifKit, AttachHandler, NULL);
    CPhidget_set_OnDetach_Handler((CPhidgetHandle) ifKit, DetachHandler, NULL);
    CPhidget_set_OnError_Handler((CPhidgetHandle) ifKit, ErrorHandler, NULL);

    CPhidgetInterfaceKit_set_OnSensorChange_Handler(ifKit, SensorChangeHandler, this);
    CPhidgetInterfaceKit_set_OnInputChange_Handler(ifKit, InputChangeHandler, this);

    CPhidget_open((CPhidgetHandle) ifKit, -1);

    #ifndef SILENT
    printf("Waiting for interface kit to be attached....");
    #endif
    
    if ((result = CPhidget_waitForAttachment((CPhidgetHandle) ifKit, 10000))) {
	CPhidget_getErrorDescription(result, &err);
	printf("Problem waiting for attachment: %s\n", err);
	throw - 1;
    }

    CPhidgetInterfaceKit_getSensorCount(ifKit, &numSensors);
    for (i = 0; i < numSensors; i++) {
	CPhidgetInterfaceKit_setSensorChangeTrigger(ifKit, i, 20);
    }    
}

InterfaceKitCallbackHandler::InterfaceKitCallbackHandler() {

}

InterfaceKitCallbackHandler::~InterfaceKitCallbackHandler()
{
    CPhidget_close((CPhidgetHandle) ifKit);
    CPhidget_delete((CPhidgetHandle) ifKit);
}
