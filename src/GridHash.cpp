#include "VCurve.h"
#include "GridHash.h"
#include "distributions.h"
#include "utils.h"
#include <iostream>

using namespace std;

GridHash::GridHash(double delta, unsigned grid_amount, unsigned dim, int k, unsigned tableSize, unsigned v_size){
	//cout << " Creating " << grid_amount << " Grids for hashing." << endl;

	if( dim!=1 && dim!=2 ){
		cout << "(!) Fatal Error: Unsupported Projection dimention given. Shutting down" << endl;
		exit(1);
	} 
	this->dim = dim;
	this->delta = delta;

	// Init 't' values - Uniformly in [0,delta)
	for(unsigned i=0; i<grid_amount; i++){
		(this->grid_t).push_back(uniform_distribution_real(0, delta-0.000001));
		// Used '0.000001' to simulate excluding delta itself from the results
	}

	// Create a Multihash Struct for each
	MultiHash *tmp;
	for(unsigned i=0; i<grid_amount; i++){
		tmp = new MultiHash(k, 1, tableSize, v_size);
		(this->mult).push_back(tmp);
	}
}

GridHash::~GridHash(){
	for(unsigned i=0; i<(this->size()); i++){
		delete (this->mult)[i];
	}
}

// Return a hashed version of the given Vector, following the 2D projection pipeline
Vector *GridHash::hash_2D(Vector *vec, double t){	
	// Create a Curve representation of the given Vector
	VCurve curve(vec);

	// Snap the Vector to the 2D Grid
	curve.snap_2D(this->delta, t);

	// Remove any resulting duplicates
	curve.remove_duplicates();

	// Vectorize the VCurve into a key Vector and return it
	return curve.vectorize();
}

// Return a hashed version of the given Vector, following the 1D projection pipeline
Vector *GridHash::hash_1D(Vector *vec, double t){
	// Create a Curve representation of the given Vector
	VCurve curve(vec);

	// Filter the curve
	curve.filter();

	// Snap the Curve to the 1D Grid
	curve.snap_1D(this->delta, t);

	// Keep only local minima and maxima
	curve.minima_maxima();

	// Vectorize the VCurve into a key Vector
	return curve.vectorize();
}

// Add the given Vector to all the Grids according to the Grid projection dimention
void GridHash::add(Vector *vec){
	Vector *key;

	// For each existing Grid
	for(unsigned i=0; i<(this->size()); i++){
		
		// Get a grid key for the Vector according to the selected projection dimention
		if( this->dim == 2 ){ key = this->hash_2D(vec, (this->grid_t)[i]); }
		else{  key = this->hash_1D(vec, (this->grid_t)[i]); }

		// Use the resulting Vector as the key to store the Initial Vector
		(this->mult)[i]->add(vec, key);

		delete key;
	}
}

// Return the nearest neighbor of the given Vector using Discrete Frechet + 2D Curve representation
ShortedList *GridHash::disc_NN_lsh(Vector *query){
	unsigned long index;
	unsigned long ID;
	unsigned id_matches;
	Bucket *bucket;
	ShortedList *list = new ShortedList(1);
	Vector *key;

	// For each existing Grid
	for(unsigned i=0; i<(this->size()); i++){
		id_matches=0;

		// Get the key Vector of the query Vector
		key = this->hash_2D(query, (this->grid_t)[i] );

		// Get the ID of the key
		ID = (((this->mult)[i]->array)[0])->g->ID(key);
		delete key;

		// Find the correct bucket based on the hash key
		index  = mod( ID , (((this->mult)[i]->array)[0])->g->tableSize );
		bucket = &( ((((this->mult)[i]->array)[0])->bucs)[index]);

		// Check the bucket and store only the nearest Vectors
		Bucket_node *cur = bucket->first;
		while( cur != nullptr ){

			// Only check Vectors with the same ID to avoid computing unnecessary distances
			if( cur->ID == ID ){
				//std::cout << " Id found" << std::endl;
				list->add( cur->data, query->discrete_frechet_distance(cur->data));
				id_matches++;
			}
			cur = cur->next;
		}

		// In case we fail to find enough ID matches, just ignore the IDs
		if( id_matches < 1 ){
			cur = bucket->first;
			while( cur != nullptr ){
				list->add( cur->data, query->discrete_frechet_distance(cur->data));
				cur = cur->next;
			}
		}
	}

	if( list->first == nullptr ){
		cout << " (!) NO NEIGHBOR FOUND (!) " << endl;
	}

	return list;
}

// Return the nearest neighbor of the given Vector using Continuous Frechet + 1D Curve representation
ShortedList *GridHash::cont_NN_lsh(Vector *query){
	unsigned long index;
	unsigned long ID;
	unsigned id_matches;
	Bucket *bucket;
	ShortedList *list = new ShortedList(1);
	Vector *key;

	// For each existing Grid
	for(unsigned i=0; i<(this->size()); i++){
		id_matches=0;

		// Get the key Vector of the query Vector
		key = this->hash_1D(query, (this->grid_t)[i] );

		// Get the ID of the key
		ID = (((this->mult)[i]->array)[0])->g->ID(key);
		delete key;

		// Find the correct bucket based on the hash key
		index  = mod( ID , (((this->mult)[i]->array)[0])->g->tableSize );
		bucket = &( ((((this->mult)[i]->array)[0])->bucs)[index]);

		// Check the bucket and store only the nearest Vectors
		Bucket_node *cur = bucket->first;
		while( cur != nullptr ){

			// Only check Vectors with the same ID to avoid computing unnecessary distances
			if( cur->ID == ID ){
				//std::cout << " Id found" << std::endl;
				list->add( cur->data, query->continuous_frechet_distance(cur->data));
				id_matches++;
			}
			cur = cur->next;
		}

		// In case we fail to find enough ID matches, just ignore the IDs
		if( id_matches < 1 ){
			cur = bucket->first;
			while( cur != nullptr ){
				list->add( cur->data, query->continuous_frechet_distance(cur->data));
				cur = cur->next;
			}
		}
	}
	return list;
}

// Add all the Vectors from the given VectorArray
void GridHash::loadVectors(VectorArray *arr){
	for(unsigned i=0; i<(this->size()); i++){
		this->add( &((arr->array)[i]) );
	}
}

// Add all the Vectors from the given AssignmentArray
void GridHash::loadVectors(AssignmentArray *arr){
	for(unsigned i=0; i<(this->size()); i++){
		this->add( &((arr->array)[i]) );
	}
}

// The amount of Grids used for Hashing
unsigned GridHash::size(){ return (this->grid_t).size(); }

// Print the Grid Parameters
void GridHash::print(){
	cout << "-------------------------------------------------" << endl;
	cout << " (i) Printing GridHash Struct: " << endl;
	cout << " Delta: " << this->delta << endl;
	cout << " 't' Values:";
	for(unsigned i=0; i<(this->size()); i++){
		cout << " " << (this->grid_t)[i];
	} cout << endl;
	cout << "-------------------------------------------------" << endl;
}
