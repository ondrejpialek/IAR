#include <stdio.h>
#include <phidget21.h>
#include "distanceSensors.h"
#include "phidget.h"

CPhidgetInterfaceKitHandle ifKit = 0;
int readings [8] = { 0 };

//callback that will run if the sensor value changes by more than the OnSensorChange trigger.
//Index - Index of the sensor that generated the event, Value - the sensor read value
int SensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int i, int v)
{
	printf("Sensor: %d > Value: %d\n", i, v);
	readings[i] = v;
	return 0;
}

//Display the properties of the attached phidget to the screen.  We will be displaying the name, serial number and version of the attached device.
//Will also display the number of inputs, outputs, and analog inputs on the interface kit as well as the state of the ratiometric flag
//and the current analog sensor sensitivity.
int display_properties(CPhidgetInterfaceKitHandle phid)
{
	int serialNo, version, numInputs, numOutputs, numSensors, triggerVal, ratiometric, i;
	const char* ptr;

	CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
	CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
	CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);

	CPhidgetInterfaceKit_getInputCount(phid, &numInputs);
	CPhidgetInterfaceKit_getOutputCount(phid, &numOutputs);
	CPhidgetInterfaceKit_getSensorCount(phid, &numSensors);
	CPhidgetInterfaceKit_getRatiometric(phid, &ratiometric);

	printf("%s\n", ptr);
	printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
	printf("# Digital Inputs: %d\n# Digital Outputs: %d\n", numInputs, numOutputs);
	printf("# Sensors: %d\n", numSensors);

	for(i = 0; i < numSensors; i++)
	{
		CPhidgetInterfaceKit_getSensorChangeTrigger (phid, i, &triggerVal);

		printf("Sensor#: %d > Sensitivity Trigger: %d\n", i, triggerVal);
	}

	return 0;
}

void initDistanceSensors() {

	int result, numSensors, i;
		const char *err;

		//Declare an InterfaceKit handle

		//create the InterfaceKit object
		CPhidgetInterfaceKit_create(&ifKit);

		//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
		CPhidget_set_OnAttach_Handler((CPhidgetHandle)ifKit, AttachHandler, NULL);
		CPhidget_set_OnDetach_Handler((CPhidgetHandle)ifKit, DetachHandler, NULL);
		CPhidget_set_OnError_Handler((CPhidgetHandle)ifKit, ErrorHandler, NULL);

		//Registers a callback that will run if the sensor value changes by more than the OnSensorChange trig-ger.
		//Requires the handle for the IntefaceKit, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
		CPhidgetInterfaceKit_set_OnSensorChange_Handler (ifKit, SensorChangeHandler, NULL);

		//open the interfacekit for device connections
		CPhidget_open((CPhidgetHandle)ifKit, -1);

		//get the program to wait for an interface kit device to be attached
		printf("Waiting for interface kit to be attached....");
		if((result = CPhidget_waitForAttachment((CPhidgetHandle)ifKit, 10000)))
		{
			CPhidget_getErrorDescription(result, &err);
			printf("Problem waiting for attachment: %s\n", err);
			throw -1;
		}

		//get the number of sensors available
		CPhidgetInterfaceKit_getSensorCount(ifKit, &numSensors);

		//Change the sensitivity trigger of the sensors
		for(i = 0; i < numSensors; i++)
		{
			CPhidgetInterfaceKit_setSensorChangeTrigger(ifKit, i, 20);
		}
}

void printInterfaceKitStatus() {
	display_properties(ifKit);
}

void closeDistanceSensors() {
	CPhidget_close((CPhidgetHandle)ifKit);
	CPhidget_delete((CPhidgetHandle)ifKit);
}



