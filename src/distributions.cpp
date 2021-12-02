#include <map>
#include <random>
#include <iomanip>
#include "distributions.h"

// Initialize random number generation used for the distributions
std::random_device rd; 	// Used to randomly seed the random number engine
std::mt19937 gen(rd());	// Standard Mersenne Twister engine seeded using the random engine from above


// Return a number following uniform distribution in the given number range 
unsigned uniform_distribution(unsigned lo, unsigned hi){
	std::uniform_int_distribution<> d(lo, hi);
	return (unsigned)(d(gen));
}

// Return a number following normal distribution with the given mean and variance
double normal_distribution(int mean, int variance){
	std::normal_distribution<double> d(mean, variance);
	return d(gen);
}

// Return a vector that has normally distributed coordinates
std::vector<double> normal_vector(int mean, int variance, unsigned v_size){
	std::vector<double> v;
	for(unsigned i=0; i<v_size; i++){
		v.push_back( normal_distribution(mean, variance));
	}
	return v;
}
