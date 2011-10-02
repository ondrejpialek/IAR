#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NANOSECONDS_PER_SECOND 1E9

int main (int argc, char *argv[])
{
	
	timespec current;
	timespec old;


	clock_gettime(CLOCK_MONOTONIC, &current);
	double diff = 0;
	int i = 0;
	while(i < 3) {
		old = current;
		clock_gettime(CLOCK_MONOTONIC, &current);
		
		double diff = ( current.tv_sec - old.tv_sec ) + ((current.tv_nsec - old.tv_nsec) / NANOSECONDS_PER_SECOND);
		

		printf("diff: %f\n", diff);
		
		sleep(1);
		i++;
	}
	
};