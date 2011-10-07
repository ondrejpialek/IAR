#ifndef INTERFACEKIT_H_
#define INTERFACEKIT_H_

#include <stdio.h>
#include <phidget21.h>
#include <time.h>

class InterfaceKitCallbackHandler {
  private:
    CPhidgetInterfaceKitHandle ifKit;
  
  public:
    void init();
    virtual void OnSensorChange(int index, int value) = 0;
    virtual void OnInputChange(int index, int value) = 0;
    
    InterfaceKitCallbackHandler();
    virtual ~InterfaceKitCallbackHandler();
};

#endif /* INTERFACEKIT_H_ */
