#include <cmath>

#define PI 3.14159265
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

#include "strategy.h"
#include "dances.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/** STEP FUNCTIONS **/

double getRand() {
    double r = rand() % 100;
    return r / 100.000;
}

void HalfHZDanceStrategy::step(double delta, bool firstRun) {
    if (timer > 0)
        timer -= delta;    
    
    if (firstRun)
        reset();
    
    switch (currentTask) {
        case Task1: {
            
            // Move back
            control->move(-10);
            
            if (timer <= 0) {
                timer = 1.8;
                currentTask = Task2;
            }
            break;
        }
        
        case Task2: {
            
            // Turn left 360
            control->turn(-50);
            
            if (timer <= 0) {
                currentTask = Task3;
                timer = 1;
            }
            break;
        }
        
        case Task3: {
            control->stop();  
            
            if (timer <= 0) {
                timer = 0.5 + 0.5 * getRand();
                currentTask = Task4;
            }           
            break;
        }
        
        case Task4: {
            control->turn(30);
            if (timer <= 0) {
                started=false;
            } 
            break;      
        }
    }
}
void HalfHZDanceStrategy::reset() {
    srand ( time(NULL) );
    
    printf("VD: 0.5\n");
    control->stop();
    currentTask = Task1;
    timer = 1.5;
    started=true;
}

double HalfHZDanceStrategy::getUtility() {    
    if (started)
        return 1;
    
    if (!(sensing->isLeftOnBlack() || sensing->isRightOnBlack())) {
        return 0;
    }
    
    double f = sensing->getFrequency();
    
    if (f > 0.3 && f < 0.7) {
        return 1;
    } else {
        return 0;
    }
}

double OneHZDanceStrategy::getUtility() {   
    if (started)
        return 1;
    
    if (!(sensing->isLeftOnBlack() || sensing->isRightOnBlack())) {
        return 0;
    }
    
    double f = sensing->getFrequency();
    
    if (f > 0.8 && f < 1.2) {
        return 1;
    } else {
        return 0;
    }
}

void OneHZDanceStrategy::reset() {
    srand ( time(NULL) );
    
    printf("VD: 1\n");
    control->stop();
    currentTask = Task1;
    timer = 1.5;
    started = true;
}

void OneHZDanceStrategy::step(double delta, bool firstRun) {
    if (timer > 0)
        timer -= delta;    
    
    if (firstRun)
        reset();
    
    switch (currentTask) {
        case Task1: {
            
            // Move back
            control->move(-10);
            
            if (timer <= 0) {
                timer = 1.8;
                currentTask = Task2;
            }
            break;
        }
        
        case Task2: {
            
            // Turn right 360
            control->turn(50);
            
            if (timer <= 0) {
                currentTask = Task3;
                timer = 1;
            }
            break;
            
        }
        
        case Task3: {
            control->stop();  
            
            if (timer <= 0) {
                timer = 0.5 + 0.5 * getRand();
                currentTask = Task4;
            }           
            break;
        }
        
        case Task4: {
            control->turn(30);
            if (timer <= 0) {
                started=false;
            } 
            break;      
        }
    }
}

double TwoHZDanceStrategy::getUtility() {   
    if (started)
        return 1;
    
    if (!(sensing->isLeftOnBlack() || sensing->isRightOnBlack())) {
        return 0;
    }
    
    double f = sensing->getFrequency();    
    
    if (f > 1.8 && f < 2.2) {
        return 1;
    } else {
        return 0;
    }
}

void TwoHZDanceStrategy::reset() {
    srand ( time(NULL) );
    
    printf("VD: 2\n");
    control->stop();
    currentTask = Task1;
    timer = 0.8;
    stopped = false;
    started=true;
}

void TwoHZDanceStrategy::step(double delta, bool firstRun) {
    if (timer > 0)
        timer -= delta;    
    
    if (firstRun)
        reset();
    
    switch (currentTask) {
        case Task1: {
            
            // Move back
            control->move(-10);
            
            if (timer <= 0) {
                timer = 1.2;
                currentTask = Task2;
                control->stop();
                stopped = true;
            }
            break;
        }
        
        case Task2: {
            
            
            // Stop
            if ((timer <= 0) && (stopped)) {
                timer = 0.8;
                stopped = false;
            }
            // Move back
            if (!stopped) {
                control->move(-10);
            }
            
            if ((timer <= 0) && (!stopped)) {
                currentTask = Task3;
                timer = 1;
            }
            break;
            
        }
        
        case Task3: {
            control->stop();  
            
            if (timer <= 0) {
                timer = 0.5 + 0.5 * getRand();
                currentTask = Task4;
            }           
            break;
        }
        
        case Task4: {
            control->turn(30);
            if (timer <= 0) {
                started=false;
            } 
            break;      
        }
    }
}

double FourHZDanceStrategy::getUtility() {   
    if (started)
        return 1;
    
    if (!(sensing->isLeftOnBlack() || sensing->isRightOnBlack())) {
        return 0;
    }
    
    double f = sensing->getFrequency();
    
    
    if (f > 3.8 && f < 4.2) {
        return 1;
    } else {
        return 0;
    }
}


void FourHZDanceStrategy::step(double delta, bool firstRun) {
    if (timer > 0)
        timer -= delta;    
    
    if (firstRun)
        reset();
    
    switch (currentTask) {
        case Task1: {
            
            // Move back
            control->move(-10);
            
            if (timer <= 0) {
                timer = 0.8;
                currentTask = Task2;
            }
            break;
        }
        
        case Task2: {
            
            // Turn left 180
            control->turn(-50);
            
            if (timer <= 0) {
                currentTask = Task3;
                timer = 1;
            }
            break;
            
        }
        
        case Task3: {
            control->stop();  
            
            if (timer <= 0) {
                timer = 0.5 + 0.5 * getRand();
                currentTask = Task4;
            }           
            break;
        }
        
        case Task4: {
            control->turn(30);
            if (timer <= 0) {
                started=false;
            } 
            break;      
        }
    }
}
void FourHZDanceStrategy::reset() {
    srand ( time(NULL) );
    
    printf("VD: 4\n");
    control->stop();
    currentTask = Task1;
    timer = 1.5;
    started=true;
}

void SixHZDanceStrategy::step(double delta, bool firstRun) {
    if (timer > 0)
        timer -= delta;    
    
    if (firstRun)
        reset();
    
    switch (currentTask) {
        case Task1: {
            
            // Move back
            control->move(-10);
            
            if (timer <= 0) {
                timer = 0.8;
                currentTask = Task2; 
            }
            break;
        }
        
        case Task2: {
            
            // Turn right 180
            control->turn(50);
            
            if (timer <= 0) {
                currentTask = Task3;
                timer = 1;
            }
            break;
        }
        
        case Task3: {
            control->stop();  
            
            if (timer <= 0) {
                timer = 0.5 + 0.5 * getRand();
                currentTask = Task4;
            }           
            break;
        }
        
        case Task4: {
            control->turn(30);
            if (timer <= 0) {
                started=false;
            } 
            break;      
        }
    }
}
void SixHZDanceStrategy::reset() {
    srand ( time(NULL) );
    
    printf("VD: 6\n");
    control->stop();
    currentTask = Task1;
    timer = 1.5;
    started=true;
}

void EightHZDanceStrategy::step(double delta, bool firstRun) {
    if (timer > 0)
        timer -= delta;    
    
    if (firstRun)
        reset();
    
    switch (currentTask) {
        case Task1: {
            
            // Move back
            control->move(-10);
            
            if (timer <= 0) {
                timer = 0.7;
                currentTask = Task2;
                
            }
            break;
        }
        
        case Task2: {
            
            // Move half distance front
            control->move(10);
            
            if (timer <= 0) {
                timer = 1;
                currentTask = Task3;
            }
            break;
        }
        
        case Task3: {
            control->stop();  
            
            if (timer <= 0) {
                timer = 0.5 + 0.5 * getRand();
                currentTask = Task4;
            }           
            break;
        }
        
        case Task4: {
            control->turn(30);
            if (timer <= 0) {
                started=false;
            } 
            break;      
        }
    }
}
void EightHZDanceStrategy::reset() {
    srand ( time(NULL) );
    
    printf("VD: 8\n");
    control->stop();
    currentTask = Task1;
    timer = 1.5;
    started=true;
}

double SixHZDanceStrategy::getUtility() {           
    if (started)
        return 1;
    
    if (!(sensing->isLeftOnBlack() || sensing->isRightOnBlack())) {
        return 0;
    }
    
    double f = sensing->getFrequency();    
    
    if (f > 5.8 && f < 6.2) {
        return 1;
    } else {
        return 0;
    }
}

double EightHZDanceStrategy::getUtility() {  
    if (started)
        return 1;
    
    if (!(sensing->isLeftOnBlack() || sensing->isRightOnBlack())) {
        return 0;
    }
    
    double f = sensing->getFrequency();    
    
    if (f > 7.8 && f < 8.2) {
        return 1;
    } else {
        return 0;
    }
}