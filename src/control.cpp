#include "control.h"
#include "motorControl.h"
#include <unistd.h>

double LEFT_MOTOR_MODIFIER = 1;
double RIGHT_MOTOR_MODIFIER = 1;
double ACCELERATION = 70.0;

void Control::ensureInitialized()
{
    if (!initialized) {
        control = new MotorControl();
        initialized = true;
    }
}

Control::Control() { }

Control::~Control() {
    delete control;
}

void Control::move(double distance)
{
    ensureInitialized();
    
    double modifier = -1;
    if (distance < 0)
        modifier = 1;
    
    control->setAcceleration(0, LEFT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(0, LEFT_MOTOR_MODIFIER * modifier * 100.00);
    control->setAcceleration(1, RIGHT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(1, RIGHT_MOTOR_MODIFIER * modifier * 100.00);
    remaining = (modifier * distance) / 10;
}

//degrees relative to current direction
void Control::turn(double degrees)
{
    ensureInitialized();
    
    double modifier = 1;
    if (degrees < 0)
        modifier = -1;
    
    control->setAcceleration(0, LEFT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(0, LEFT_MOTOR_MODIFIER * 100.00 * modifier);
    control->setAcceleration(1, RIGHT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(1, -1 * RIGHT_MOTOR_MODIFIER * 100.00 * modifier);
    remaining = (degrees * modifier) / 290;
}

void Control::stop()
{
    ensureInitialized();
    
    control->setAcceleration(0, LEFT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(0, 0.00);
    control->setAcceleration(1, RIGHT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(1, 0.00);
}

void Control::controlTick(double diff)
{
    remaining -= diff;
    if (remaining < 0) {
        stop();
    }
}
