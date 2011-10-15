#include <cmath>

#define PI 3.14159265
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

#include "strategy.h"
#include "dances.h"

/** STEP FUNCTIONS **/

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
	    }
	    break;
      }
      
      	  case Task3: {

	    // DO NOTHING
	    break;
      }
  }
}
void HalfHZDanceStrategy::reset() {
    control->stop();
    currentTask = Task1;
    timer = 1.5;

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
	    }
	    break;

      }
      
         case Task3: {

	    // DO NOTHING
	    break;
      }
  }
}
void OneHZDanceStrategy::reset() {
    control->stop();
    currentTask = Task1;
    timer = 1.5;

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
	    }
	    break;

      }
      
          case Task3: {

	    // DO NOTHING
	    break;
      }
  }
}
void TwoHZDanceStrategy::reset() {
    control->stop();
    currentTask = Task1;
    timer = 0.8;
    stopped = false;
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
	    }
	    break;

      }
         case Task3: {

	    // DO NOTHING
	    break;
      }
  }
}
void FourHZDanceStrategy::reset() {
    control->stop();
    currentTask = Task1;
    timer = 1.5;

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
	      
	    }
	    break;
      }
          case Task3: {

	    // DO NOTHING
	    break;
      }
  }
}
void SixHZDanceStrategy::reset() {
    control->stop();
    currentTask = Task1;
    timer = 1.5;

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
	      currentTask = Task3;
	      
	    }
	    break;
      }
      
          case Task3: {

	    // DO NOTHING
	    break;
      }
  }
}
void EightHZDanceStrategy::reset() {
    control->stop();
    currentTask = Task1;
    timer = 1.5;

}


/** GET UTILITY FUNCTIONS **/

double HalfHZDanceStrategy::getUtility() {    
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

double TwoHZDanceStrategy::getUtility() {   
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

double FourHZDanceStrategy::getUtility() {   
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

double SixHZDanceStrategy::getUtility() {           
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