#include "timer.h"

void Timer::tic(){
	this->start = clock();
}

double Timer::toc(){
	return ((double)(clock() - (this->start))) / CLOCKS_PER_SEC;
}
