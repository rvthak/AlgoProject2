#ifndef DISTRIBUTIONS
#define DISTRIBUTIONS

#include <vector>

	unsigned uniform_distribution(unsigned lo, unsigned hi);
	double normal_distribution(int mean, int variance);
	
	std::vector<double> normal_vector(int mean, int variance, unsigned v_size);

#endif