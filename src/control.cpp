#include "control.h"
#include "motorControl.h"
#include <unistd.h>

double LEFT_MOTOR_MODIFIER = 1;
double RIGHT_MOTOR_MODIFIER = 1;
double ACCELERATION = 60.0;

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

void Control::moveSlow(double distance)
{
    ensureInitialized();
    
    double modifier = -1;
    if (distance < 0)
        modifier = 1;
    
    control->setAcceleration(0, LEFT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(0, LEFT_MOTOR_MODIFIER * modifier * 40.00);
    control->setAcceleration(1, RIGHT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(1, RIGHT_MOTOR_MODIFIER * modifier * 40.00);
    remaining = (modifier * distance) / 10;
}

//degrees relative to current direction
void Control::turn(double degrees, double velocity)
{    
    double modifier = 1;
    if (degrees < 0)
        modifier = -1;
    
    control->setAcceleration(0, LEFT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(0, LEFT_MOTOR_MODIFIER * velocity * modifier);
    control->setAcceleration(1, RIGHT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(1, -1 * RIGHT_MOTOR_MODIFIER * velocity * modifier);
    remaining = (degrees * modifier) / 290;
}

//degrees relative to current direction
void Control::turn(double degrees)
{
    ensureInitialized();
    turn(degrees, 100.00);
}

//degrees relative to current direction
void Control::turnSlow(double degrees)
{
    ensureInitialized();
    turn(degrees, 40.00);
}

//degrees relative to current direction
void Control::turnSingle(double degrees)
{
    ensureInitialized();
    
    double modifier = 1;
    if (degrees < 0)
        modifier = -1;
    
    double l = LEFT_MOTOR_MODIFIER;
    double r = RIGHT_MOTOR_MODIFIER;
    
    if (modifier == 1) {
        r = 0;
    } else {
        l = 0;
    }
    
    control->setAcceleration(0, LEFT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(0, l * 40.00 * modifier);
    control->setAcceleration(1, RIGHT_MOTOR_MODIFIER * ACCELERATION);
    control->setVelocity(1, -1 * r * 40.00 * modifier);
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
