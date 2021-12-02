#include "utils.h"
#include "hash_fun.h"
#include "distributions.h"
#include <math.h>
#include <vector>
#include <numeric>
#include <iostream>
#include <functional>

#define M_VALUE 1800164
#define W_VALUE 9 // in [2,6] but increase further for range queries with large R
#define MAX_R 6

H::H(unsigned v_size){
	this->t = uniform_distribution(0, W_VALUE-1);
	this->w = W_VALUE;
	this->v = normal_vector(0,1,v_size);
}

// Calculate the hash key of the given Vector
// Based on math type, slide 19 of "nnCluster.pdf"
double H::hash(Vector *p){
	return floor( (dot_product(p->vec, this->v) + (this->t)) / (double)(this->w) );
}

//------------------------------------------------------------------------------------------------------------------

// Create the random hash functions "h" and the random values "r" and store them
G::G(int k, unsigned tableSize, unsigned v_size){
	this->M = M_VALUE;
	this->k = k;
	this->tableSize = tableSize;

	//std::cout << " Created G containing: " << std::endl;

	// Generate uniform random values for k "r"s
	this->r = new unsigned[k];
	if( this->r == nullptr ){
		std::cout << "\033[31;1m (!) Fatal Error:\033[0m Memory : Failed to allocate memory for 'r' values." << std::endl;
		exit(1);
	}
	for(int i=0; i<k; i++){ 
		(this->r)[i] = uniform_distribution(1, MAX_R); 
		//std::cout << " r[" << i << "] = " << (this->r)[i] << std::endl;
	}

	// Generate k random "h" functions
	if( (this->h = new H*[k]) == nullptr ){
		std::cout << "\033[31;1m (!) Fatal Error:\033[0m Memory : Failed to allocate memory for H[]." << std::endl;
		exit(1);
	}
	for(int i=0; i<k; i++){
		if( ((this->h)[i] = new H(v_size)) == nullptr ){
			std::cout << "\033[31;1m (!) Fatal Error:\033[0m MultiHash Built : Failed to allocate memory for Hi." << std::endl;
			exit(1);
		}
	}
}

G::~G(){
	for(unsigned i=0; i<(this->k); i++){
		delete (this->h)[i];
	}
	delete [] (this->h);
	delete [] this->r;
}

// Calculate the hash key of the given Vector (Sum-Based)
unsigned long G::hash(Vector *p){
	return mod( this->ID(p) , this->tableSize );
}

// Calculate the ID of the given Vector (Sum-Based)
// Based on the math type on slide 21 of "nnCluster.pdf"
unsigned long G::ID(Vector *p){
	double sum=0;
	for(unsigned i=0; i<(this->k); i++){
		sum += (this->r)[i] * ((this->h)[i]->hash(p));
	}
	return mod(sum, this->M);
}

//------------------------------------------------------------------------------------------------------------------

F::F(unsigned k, unsigned dimensions, unsigned table_size){
	this->k = k;
	this->dimensions = dimensions;
	this->table_size = table_size;

	// Generate k random "h" functions
	if((this->h = new H*[k]) == nullptr)
	{
		std::cout << "\033[31;1m (!) Fatal Error:\033[0m Memory : Failed to allocate memory for H[]." << std::endl;
		exit(1);
	}

	for(unsigned i = 0; i < k; i++)
	{
		if(((this->h)[i] = new H(dimensions)) == nullptr)
		{
			std::cout << "\033[31;1m (!) Fatal Error:\033[0m MultiHash Built : Failed to allocate memory for Hi." << std::endl;
			exit(1);
		}
	}
}

F::~F(){
	for(unsigned i = 0; i < (this->k); i++){
		delete (this->h)[i];
	}
}

unsigned F::hash(Vector *p){
	unsigned hash_key_from_array, final_hash_key;
	std::vector<unsigned> bit_array;
	unsigned h_key, bit;

	for(unsigned i = 0; i < (this->k); i++)
	{
		h_key = (this->h)[i]->hash(p);
		bit = generate_bit_from_h_key(h_key);
		bit_array.push_back(bit);
	}

	hash_key_from_array = convert_bit_array_to_decimal(bit_array);
	final_hash_key = hash_key_from_array % (this->table_size);

	return final_hash_key;
}
