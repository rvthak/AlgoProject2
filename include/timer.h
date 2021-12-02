#ifndef TIMER
#define TIMER

#include <ctime>

// Struct used for timing
struct Timer{
	clock_t start;	// The curent start time (the moment tik() was called)

	void tic();		// Called before the code we want to time
	double toc();	// Called after the code we want to time - Returns the time in Seconds
};

#endif
