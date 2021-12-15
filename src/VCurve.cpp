#include "VCurve.h"
#include "iostream"
#include "utils.h"

using namespace std;

// A Large prime + a decimal part - just because I like primes
#define PADDING_NUM 1000003.700171

// A small constant used as the limit for filtering
#define E_CONSTANT 0.1

// Convert the given Vector to a VCurve
// Uses sampling rate = 1 unit of time
VCurve::VCurve(Vector *v){

	for(unsigned i=0; i<(v->size()); i++){
		(this->x).push_back(i);
		(this->y).push_back( (v->vec)[i] );
	}
	this->vec = v;
	this->max_vec_size = v->size();
}

// Converts the VCurve to a Vector and returns a pointer to it
// Uses concat method
Vector *VCurve::vectorize(){
	Vector *vec = new Vector;

	// Just Concatinate the pairs
	for(unsigned i=0; i<(this->size()); i++){
		(vec->vec).push_back( (this->x)[i] );
		(vec->vec).push_back( (this->y)[i] );
	}
	return vec;
}

// Filter-out consecutive points that show very little volatility
void VCurve::filter(){
	// The filtered vectors
	vector<double> x, y;

	// Add the first point - edge case
	x.push_back( (this->x)[0] );
	y.push_back( (this->y)[0] );

	// Iterate through the VCurve triplets and filter-out any insignificant values
	for(unsigned i=1; i<((this->size())-1); i++){

		if( abs( (this->y)[i-1] - (this->y)[i] ) <= E_CONSTANT ||
		    abs( (this->y)[i+1] - (this->y)[i] ) <= E_CONSTANT ){
			continue;
		}
		x.push_back( (this->x)[i] );
		y.push_back( (this->y)[i] );
	}

	// Add the last point - edge case
	x.push_back( (this->x)[(this->size())-1] );
	y.push_back( (this->y)[(this->size())-1] );

	// Write the new vectors on the VCurve
	this->x = x;
	this->y = y;
}

// Filter-out non-important points, keep only local minima and maxima
// By definition (Slides): remove any vi s.t. vi e [ min{vi-1, vi+1}, max{vi-1, vi+1} ] 
// We don't check the first and the last points since they can't satisfy the definition type
void VCurve::minima_maxima(){
	std::vector<double> x, y;
	// Add the first point - edge case
	x.push_back( (this->x)[0] );
	y.push_back( (this->y)[0] );

	for(unsigned i=1; i<((this->size())-1); i++){

		if( belongs( (this->x)[i], min((this->x)[i-1],(this->x)[i+1]), max((this->x)[i-1],(this->x)[i+1])) ){
			continue;
		}
		x.push_back( (this->x)[i] );
		y.push_back( (this->y)[i] );
	}
	// Add the last point - edge case
	x.push_back( (this->x)[(this->size())-1] );
	y.push_back( (this->y)[(this->size())-1] );

	// Write the new vectors on the VCurve
	this->x = x;
	this->y = y;
}

// Remove any duplicate points that may result from grid snapping
void VCurve::remove_duplicates(){
	std::vector<double> x, y;
	x.push_back( (this->x)[0] );
	y.push_back( (this->y)[0] );

	for(unsigned i=1; i<(this->size()); i++){
		if( (this->x)[i]==(this->x)[i-1] && (this->y)[i]==(this->y)[i-1] ){ 
			//cout << " REOVED" << endl;
			continue; }
		x.push_back( (this->x)[i] );
		y.push_back( (this->y)[i] );
	}
	// Write the new vectors on the VCurve
	this->x = x;
	this->y = y;
}

// Snap the VCurve to a 2D grid defined by the given 'delta' and 't' values
void VCurve::snap_2D(double delta, double t){

	// Pass through all the existing points and snap them on the 2D grid one by one
	for(unsigned i=0; i<(this->size()); i++){
		(this->x)[i] = snap((this->x)[i], delta, t);
		(this->y)[i] = snap((this->y)[i], delta, t);
	}
}

// Snap the VCurve to a 1D grid defined by the given 'delta' and 't' values
// Snap only the 'y' values
void VCurve::snap_1D(double delta, double t){

	// Pass through all the existing points and snap them on the 2D grid one by one
	for(unsigned i=0; i<(this->size()); i++){
		(this->y)[i] = snap((this->y)[i], delta, t);
	}
}

// Add the needed amout of padding to get the curve to "max_vec_size"
void VCurve::add_padding(){
	unsigned vec_len=this->size();

	for(unsigned i=0; i<((this->max_vec_size)/2); i++){

		if( i >= vec_len ){
			(this->x).push_back(PADDING_NUM);
			(this->y).push_back(PADDING_NUM);
		}
	}
}

// Returns the current VCurve length
unsigned VCurve::size(){ return (this->x).size(); }

// Prints the VCurve in tuples
void VCurve::print(){
	cout << " [";
	for(unsigned i=0; i<(this->size()); i++){
		cout << "(" << (this->x)[i] << "," << (this->y)[i] << "), ";
	}
	cout << endl;
}
