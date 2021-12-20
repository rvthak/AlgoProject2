#include "VCurve.h"
#include "iostream"
#include "utils.h"

using namespace std;

// A Large prime + a decimal part - just because I like primes
#define PADDING_NUM 1000003.700171

#define LEFT_MOVE 0
#define DIAG_MOVE 1
#define DOWN_MOVE 2

unsigned max_move(double left, double diag, double down);
double bounds(double **c, int pi, int qi);

//------------------------------------------------------------------------------------------------------------------

VCurve::VCurve(){
	this->vec = nullptr;
	this->max_vec_size = 0;
}

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

VCurve::VCurve(std::vector<double> *v){
	for(unsigned i=0; i<(v->size()); i++){
		(this->x).push_back(i);
		(this->y).push_back( (*v)[i] );
	}
	this->vec = nullptr;
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

	//cout << vec->size() << endl;
	return vec;
}

// Convert only the requested dimention into a vector
std::vector<double> VCurve::vectorize_dim(unsigned dim){
	std::vector<double> v, *ptr;

	if( dim == 0 ){ ptr = &(this->x); }
	else{ ptr = &(this->y); }

	for(unsigned i=0; i<(this->size()); i++){
		v.push_back( (*ptr)[i] );
	}
	return v; 
}

// Convert only the requested dimention into a vector
// Stop the conversion and return if the given limit length was surpassed
std::vector<double> VCurve::vectorize_dim(unsigned dim, unsigned limit){
	std::vector<double> v, *ptr;

	if( dim == 0 ){ ptr = &(this->x); }
	else{ ptr = &(this->y); }

	for(unsigned i=0; i<(this->size()); i++){
		v.push_back( (*ptr)[i] );
		if( v.size() == limit ){ break; }
	}
	return v; 
}

// Convert only the requested dimention into a vector
// Fill the end of the vector with the last element
std::vector<double> VCurve::vectorize_fill(unsigned dim, unsigned size){
	std::vector<double> v, *ptr;

	if( dim == 0 ){ ptr = &(this->x); }
	else{ ptr = &(this->y); }

	for(unsigned i=0; i<(this->size()); i++){
		v.push_back( (*ptr)[i] );
	}

	for(unsigned i=(this->size()); i<size; i++){
		v.push_back( v[(this->size())-1] );
	}
	return v; 
}

// Convert the Curve to a vector of the given size,
// try to not lose too much information
// We know that any given curve cannot be smaller than the given size, so we
// just need to cut/filter it down to the required size somehow
std::vector<double> VCurve::vectorize_size(unsigned size){

	// If we are lucky and we already have the correct size, just return it as it is
	if( (this->size()) == size ){ return vectorize_dim(1); }

	// Else we will need to shorted the curve length

	// Try filtering
	this->filter(0.5);
	if( (this->size()) == size ){ return vectorize_dim(1); }
	if( (this->size()) < size ){ return vectorize_fill(1,size); }

	// Else just cut the end off
	return vectorize_dim(1, size);
}

// Filter-out consecutive points that show very little volatility
void VCurve::filter(double e){
	// The filtered vectors
	vector<double> x, y;

	// Add the first point - edge case
	x.push_back( (this->x)[0] );
	y.push_back( (this->y)[0] );

	// Iterate through the VCurve triplets and filter-out any insignificant values
	for(unsigned i=1; i<((this->size())-1); i++){

		if( abs( (this->y)[i-1] - (this->y)[i] ) <= e &&
		    abs( (this->y)[i+1] - (this->y)[i] ) <= e ){
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

		if( belongs( (this->y)[i], min((this->y)[i-1],(this->y)[i+1]), max((this->y)[i-1],(this->y)[i+1])) ){
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
			//cout << " REMOVED" << endl;
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

	for(unsigned i=0; i<(this->max_vec_size); i++){

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
	cout << " [ ";
	for(unsigned i=0; i<(this->size()); i++){
		cout << "(" << (this->x)[i] << "," << (this->y)[i] << ") ";
	}
	cout << "]" << endl;
}

void VCurve::set_max_size(unsigned size){ this->max_vec_size = size; }

// Returns true if the Curve is currently empty
bool VCurve::isEmpty(){ return ((this->max_vec_size) == 0); }

// Add the given point to the Curve
void VCurve::push_back(double xi, double yi){
	(this->x).push_back(xi);
	(this->y).push_back(yi);
}

//------------------------------------------------------------------------------------------------------------------

// Calculate the Mean Discrete Frechet curve of the two given Curves
VCurve *meanCurve(VCurve *p, VCurve *q){

	// In case only one curve was given => return it as the mean
	if( (q->size()) == 0 ){ return p; }

	// Start by calculating the discrete frechet distance between the two vectors
	// Exact similar process with Vector::discrete_frechet_distance()
	double **c    = new_2D_table(p->size(), q->size());
	double **dist = new_2D_table(p->size(), q->size());

	// Calculate the distances and store them
	for(unsigned i=0; i<(p->size()); i++){	// For every 2D point in Vector 'p'
		for(unsigned j=0; j<(q->size()); j++){	// For every 2D point in Vector 'q'
			dist[i][j] = euclidian_distance((p->x)[i],(p->y)[i], (q->x)[j],(q->y)[j]);
		}
	}

	// Use the distances to calculate the Frechet distance
	for(unsigned i=0; i<(p->size()); i++){
		for(unsigned j=0; j<(p->size()); j++){
			if( (i==0)&&(j==0) ){
				c[i][j] = dist[i][j];
			} else if( i==0 ){
				c[i][j] = max( c[i][j-1], dist[i][j] );
			} else if( j==0 ){
				c[i][j] = max( c[i-1][j], dist[i][j] );
			} else {
				c[i][j] = max( min( min( c[i-1][j],c[i-1][j-1] ), c[i][j-1] ), dist[i][j] );
			}
		}
	}

	// Now we can backtrack the table to get an optimal traversal
	std::vector<int> trav_p, trav_q;

	// We start from the solution (the final cell of the table)
	// and work our way back to the first cell, while storing the steps
	int pi=(p->size())-1, qi=(q->size())-1, move;

	trav_p.push_back(pi);
	trav_q.push_back(qi);
	
	while( pi!=0 && qi!=0 ){
		move = max_move( bounds(c,pi-1,qi), bounds(c,pi-1,qi-1), bounds(c,pi,qi-1) );

		// Store the Curve indexes per traversal step
		if( move == LEFT_MOVE ){ // left
			pi--;
		}
		else if( move == DIAG_MOVE ){ // diag
			pi--; qi--;
		}
		else{ // down == DOWN_MOVE
			qi--;
		}
		trav_p.push_back(pi);
		trav_q.push_back(qi);
	}

	// Free the tables
	delete_2D_table(c, p->size()); delete_2D_table(dist, p->size());
	
	// Based on the resulting traversal => average the step points to the mean vector
	VCurve *mean = new VCurve();

	for(unsigned i=0; i<(trav_p.size()); i++){
		mean->push_back( average( trav_p[i], trav_q[i] ), 
			            average( (p->y)[trav_p[i]], (q->y)[trav_q[i]] ));
	}

	mean->set_max_size(trav_p.size());
	return mean;
}

// Return a code that indicates which direction has the largest value
unsigned max_move(double left, double diag, double down){
	if( (left >= diag) && (left >= down) ){ return LEFT_MOVE; }
	if( (diag >= left) && (diag >= down) ){ return DIAG_MOVE; }
	else{ return DOWN_MOVE; }
}

// Ensure that the given index is inside table and then return it
// if it is not in table bounds, return zero
double bounds(double **c, int pi, int qi){
	if( (pi < 0) || (qi < 0) ){ return 0; }
	return c[pi][qi];
}
