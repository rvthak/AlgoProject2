#ifndef VCURVE
#define VCURVE

#include <vector>
#include "Vector.h"

// Represents a Vector as a Curve considering that sampling rate (x-axis values) is 1 unit
struct VCurve{
	Vector *vec;
	std::vector<double> x, y;
	unsigned max_vec_size;
	
	VCurve(Vector *v);

	// VCurve Mutators
	void filter();				// Filter-out consecutive points that show very little volatility
	void minima_maxima();		// Filter-out non-important points, keep only local minima and maxima
	void remove_duplicates();	// Remove any duplicate points that may result from grid snapping

	// VCurve Grid Snapping
	void snap_2D(double delta, double t);
	void snap_1D(double delta, double t);

	// Utils
	void add_padding();		// Add the needed amout of padding to get the curve to "max_vec_size"
	Vector *vectorize();	// Converts the VCurve to a Vector and returns a pointer to it
	unsigned size();		// Returns the current VCurve length
	void print();			// Prints the VCurve	
};

// We work under the assumption that all the Vectors of our dataset share the same length
// "max_vec_size": It corresponds to the Vector complexity * 2 

// A.ii  Pipeline: snap_2D -> remove_duplicates -> padding 
// A.iii Pipeline: filter -> snap_1D -> minima_maxima -> padding 
// ( In the second case I think minima_maxima also filters-out any duplicates as well )

#endif
