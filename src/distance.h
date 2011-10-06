
#ifndef DISTANCE_H_
#define DISTANCE_H_

#define NANOSECONDS_PER_SECOND 1E9

class Sensing : public InterfaceKitCallbackHandler {
  public:
    virtual void SensorChangeHandler(int index, int value) = 0;
    virtual void InputChangeHandler(int index, int value) = 0;
}

void initDistance();
void closeDistance();

int getLeftDistance();

int getRightDistance();

bool getFrontWhisker();

bool getBackWhisker();

#endif /* DISTANCE_H_ */
