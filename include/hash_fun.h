#ifndef HASH_FUNCTIONS
#define HASH_FUNCTIONS

#include "Vector.h"

// Hash Function "h"
struct H{
	std::vector<double> v;	// Vector based on Normal Distribution
	double t;				// Adds Slight random variation (Uniformly in [0,w) )
	unsigned w; 			// "Window" - NOT zero

	H(unsigned v_size);
	double hash(Vector *p);
};

// Hash Function "g" (Randomized on initialization) - Calculated based on sub-hash-functions "h"
struct G{
	unsigned long M;		// Large Prime Number
	unsigned tableSize;		// The size of the TargetHash table (heuristic choice)
	unsigned k;				// The amount of "h" hash functions

	// We create both ri and hi randomly on struct init, so we need to store them
	unsigned *r;	// The "ri" values
	H **h;			// The "hi" functions

	G(int k, unsigned tableSize, unsigned v_size);
	~G();
	unsigned long hash(Vector *p);
	unsigned long ID(Vector *p);
};

// Struct used for Hypercube hashing
struct F {
	unsigned dimensions;	// The Dimentions of the Hypercube
	unsigned table_size;	// The tableSize which comes from dimensions
	unsigned k;				// The amount of "h" hash functions
	H **h;		            // Array of h values

	F(unsigned k, unsigned dimensions, unsigned table_size);
	~F();

	unsigned hash(Vector *p);
};

#endif
