#ifndef VCURVE
#define VCURVE

#include <vector>
#include "Vector.h"

// A small constant used as the limit for filtering
#define E_CONSTANT 0.3

// Represents a Vector as a Curve considering that sampling rate (x-axis values) is 1 unit
struct VCurve{
	Vector *vec;
	std::vector<double> x, y;
	unsigned max_vec_size;
	
	VCurve();
	VCurve(Vector *v);
	VCurve(std::vector<double> *v);

	// VCurve Mutators
	void filter(double e = E_CONSTANT);	// Filter-out consecutive points that show very little volatility
	void minima_maxima();				// Filter-out non-important points, keep only local minima and maxima
	void remove_duplicates();			// Remove any duplicate points that may result from grid snapping
	void add_padding();					// Add the needed amout of padding to get the curve to "max_vec_size"

	// VCurve Grid Snapping
	void snap_2D(double delta, double t);
	void snap_1D(double delta, double t);

	Vector *vectorize();	// Converts the VCurve to a Vector and returns a pointer to it
	std::vector<double> vectorize_dim(unsigned dim);
	std::vector<double> vectorize_dim(unsigned dim, unsigned limit);
	std::vector<double> vectorize_fill(unsigned dim, unsigned size);
	std::vector<double> vectorize_size(unsigned size);

	// Utils
	unsigned size();	// Returns the current VCurve length
	void print();		// Prints the VCurve
	bool isEmpty();		// Returns true if the Curve is currently empty
	void push_back(double xi, double yi);	// Add the given point to the Curve
	void set_max_size(unsigned size);		// Setter for 'max_vec_size'
};

// Return the discrete frechet mean curve of the two given curves
VCurve *meanCurve(VCurve *p, VCurve *q);

// We work under the assumption that all the Vectors of our dataset share the same length

// A.ii  Pipeline: snap_2D -> remove_duplicates -> padding 
// A.iii Pipeline: filter -> snap_1D -> minima_maxima -> padding 
// ( In the second case I think minima_maxima also filters-out any duplicates as well )

#endif
