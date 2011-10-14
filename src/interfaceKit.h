#ifndef INTERFACEKIT_H_
#define INTERFACEKIT_H_

#include <stdio.h>
#include <phidget21.h>
#include <time.h>

enum Sensors { LeftIR = 1, RightIR, RightBottomLight, LeftBottomLight, RightFrontLight, LeftFrontLight  };

class InterfaceKitCallbackHandler {
    private:
        bool initialized;
        CPhidgetInterfaceKitHandle ifKit;
        
    protected:
        void ensureInitialized();
        void virtual initialize();
        
    public:
        virtual void OnSensorChange(int index, int value) = 0;
        virtual void OnInputChange(int index, int value) = 0;
        
        InterfaceKitCallbackHandler();
        virtual ~InterfaceKitCallbackHandler();
};

#endif /* INTERFACEKIT_H_ */
