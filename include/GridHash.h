#ifndef GRIDHASH
#define GRIDHASH

#include <vector>
#include "Vector.h"
#include "hash_lsh.h"
#include "shortedList.h"

#define DIVISION_SIZE_GRID_1D 3
#define DIVISION_SIZE_GRID_2D 4

struct GridHash{
	unsigned dim; // The projection dimention

	// Grid Snapping
	double delta;				// The Grid snap-points distance ~ Grid resolution
	std::vector<double> grid_t;	// One 't' shift value for each Grid

	// Vector Storage - One MultiHash for each Grid
	std::vector<MultiHash *> mult;

	GridHash(double delta, unsigned grid_amount, unsigned dim, int k, unsigned tableSize, unsigned v_size);
	~GridHash();

	void add(Vector *vec);

	Vector *hash_2D(Vector *vec, double t);	// Return a hashed version of the given Vector, following the 2D projection pipeline
	Vector *hash_1D(Vector *vec, double t);	// Return a hashed version of the given Vector, following the 1D projection pipeline
	// These hash functions return new Vectors and don't mutate the given ones. Remember to free the returned Vectors after use

	ShortedList *disc_NN_lsh(Vector *query);	// Return the nearest neighbor of the given Vector using Discrete Frechet + 2D Curve representation
	ShortedList *cont_NN_lsh(Vector *query);	// Return the nearest neighbor of the given Vector using Continuous Frechet + 1D Curve representation
	List *range_search(Vector *query, double R, VecDist distfun);	// Range search in the Grid struct

	void loadVectors(VectorArray *arr);			// Add all the Vectors from the given VectorArray
	void loadVectors(AssignmentArray *arr);		// Add all the Vectors from the given AssignmentArray

	unsigned size();	// The amount of Grids used for Hashing
	void print();		// Print the Grid Parameters

	// Print the Average bucket size for each of the existing MultiHashes
	void averageBucketSize();
};

#endif
