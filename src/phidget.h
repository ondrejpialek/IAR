#ifndef PHIDGET_H_
#define PHIDGET_H_

#include <phidget21.h>

int AttachHandler(CPhidgetHandle MC, void *userptr);
int DetachHandler(CPhidgetHandle MC, void *userptr);
int ErrorHandler(CPhidgetHandle MC, void *userptr, int ErrorCode, const char *Description);

#endif /* PHIDGET_H_ */
