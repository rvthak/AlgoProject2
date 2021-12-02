#include "timer.h"
#include "utils.h"
#include "hash_lsh.h"
#include <iostream>


// Create a Hash table with a randomized "g" hash function
HashTable::HashTable(int k, unsigned tableSize, unsigned v_size){

	// Allocate the Bucket Array
	this->bucs = new Bucket[tableSize];
	if( this->bucs == nullptr ){
		std::cout << "\033[31;1m (!) Fatal Error:\033[0m Memory : Failed to allocate memory for Bucket." << std::endl;
		exit(1);
	}
	this->size = tableSize;

	// Create the randomized hash function
	this->g = new G(k, tableSize, v_size);
	if( this->g == nullptr ){
		std::cout << "\033[31;1m (!) Fatal Error:\033[0m Memory : Failed to create G hash function." << std::endl;
		exit(1);
	}
}

HashTable::~HashTable(){
	delete [] this->bucs; 
	delete this->g;
}

// Add the given Vector to "this" Hash Table
int HashTable::add(Vector *vec){
	unsigned long id = this->g->ID(vec);
	unsigned long key = mod(id, g->tableSize);
	//std::cout << " KEY: " << key << ", ID: " << id << std::endl;
	return (this->bucs)[key].add(vec, id);
}

// Print only the buckets + the amount of Vectors stored on each one
void HashTable::preview(){
	for(unsigned i=0; i<(this->size); i++){
		std::cout << " Bucket: " << i+1 << ", Vectors Stored: " << (this->bucs)[i].size << std::endl;
	}
}

// Return the Average amount of Vectors per Bucket in the Hash Table
double HashTable::averageBucketSize(){
	double sum=0;
	for(unsigned i=0; i<(this->size); i++){
		sum += (this->bucs)[i].size;
	}
	return sum/(this->size);
}

//------------------------------------------------------------------------------------------------------------------

// Allocate L hash tables, and set their hash functions to use k "h" sub-hash-functions
MultiHash::MultiHash(int k, int L, unsigned tableSize, unsigned v_size){
	//std::cout << " Creating Multihash > k: " << k << ", L: " << L << ", TableSize: " << tableSize << ", VectorSize: " << v_size << std::endl;
	// Allocate the Hash tables
	if( (this->array = new HashTable*[L]) == nullptr ){ 
		std::cout << "\033[31;1m (!) Fatal Error:\033[0m MultiHash Built : Failed to allocate memory." << std::endl;
		exit(1);
	}
	for(int i=0; i<L; i++){
		if( ((this->array)[i] = new HashTable(k, tableSize, v_size)) == nullptr ){
			std::cout << "\033[31;1m (!) Fatal Error:\033[0m MultiHash Built : Failed to allocate memory." << std::endl;
			exit(1);
		}
	}
	this->amount = L;
}

MultiHash::~MultiHash(){
	for(unsigned i=0; i<(this->amount); i++){
		delete (this->array)[i];
	}
	delete [] (this->array);
}

// Add the given Vector on every Hash Table
void MultiHash::add(Vector *vec){
	for(unsigned i=0; i<this->amount; i++){
		if( ((this->array)[i])->add(vec) ){
			std::cout << "\033[31;1m (!) Fatal Error:\033[0m Hash Table Built : Failed to add Vector to Hash Table." << std::endl;
			exit(1);
		}
	}
}

// Load all the Vectors of the VectorArray in the MultiHash Struct
void MultiHash::loadVectors(VectorArray *arr){
	// Add each vector to all the Hash tables
	for(unsigned i=0; i<(arr->size); i++){
		this->add( &((arr->array)[i]) );
	}
}

// Load all the Vectors of the AssignmentArray in the MultiHash Struct
void MultiHash::loadVectors(AssignmentArray *arr){
	// Add each vector to all the Hash tables
	for(unsigned i=0; i<(arr->size); i++){
		this->add( &((arr->array)[i]) );
	}
}

// LSH search for the k approximate Nearest Neighbors of the given query Vector
ShortedList *MultiHash::kNN_lsh(Vector *query, unsigned k){
	unsigned long key;
	unsigned long ID;
	unsigned id_matches=0;
	Bucket *bucket;
	ShortedList *list = new ShortedList(k);

	std::cout << " > Query id: " << query->id << std::endl;
	// For each existing Hash Table
	for(unsigned i=0; i<(this->amount); i++){
		//std::cout << "\tSearching HT[" << i+1 << "]" << std::endl;
		id_matches=0;

		// Calculate the ID of the query vector
		ID = ((this->array)[i])->g->ID(query);

		// Find the correct bucket based on the hash key
		key    = mod( ID , ((this->array)[i])->g->tableSize );
		bucket = &((((this->array)[i])->bucs)[key]);

		// Check the bucket and store only the nearest Vectors
		Bucket_node *cur = bucket->first;
		while( cur != nullptr ){

			// Only check Vectors with the same ID to avoid computing unnecessary distances
			if( cur->ID == ID ){
				//std::cout << " Id found" << std::endl;
				list->add( cur->data, query->l2(cur->data));
				id_matches++;
			}
			cur = cur->next;
		}

		// In case we fail to find enough ID matches, just ignore the IDs
		if( id_matches > k ){
			cur = bucket->first;
			while( cur != nullptr ){
				list->add( cur->data, query->l2(cur->data));
				cur = cur->next;
			}
		}
	}
	return list;
}

// Range search for the k approximate Nearest Neighbors of the given query Vector
List *MultiHash::range_search(Vector *query, double R){
	long key;
	long ID;
	double dist;
	Bucket *bucket;
	List *list = new List();

	// For each existing Hash Table
	for(unsigned i=0; i<(this->amount); i++){

		// Calculate the ID of the query vector
		ID = ((this->array)[i])->g->ID(query);

		// Find the correct bucket based on the hash key
		key    = mod( ID , ((this->array)[i])->g->tableSize );
		bucket = &((((this->array)[i])->bucs)[key]);

		// Check the bucket and store only the nearest Vectors
		Bucket_node *cur = bucket->first;
		while( cur != nullptr ){
			if( cur->ID == ID ){
				if( (dist = query->l2(cur->data)) <= R ){
					list->add( cur->data);
				}
			}
			cur = cur->next;
		}
	}
	
	return list;
}

// Print the Multihash struct
void MultiHash::print(){
	for(unsigned i=0; i<(this->amount); i++){
		(this->array)[i]->preview();
		std::cout << std::endl; 
	}
}

// Return the Average amount of Vectors per Bucket between all the Hash Tables
double MultiHash::averageBucketSize(){
	double sum=0;
	for(unsigned i=0; i<(this->amount); i++){
		sum += (this->array)[i]->averageBucketSize();
	}
	return sum/(this->amount);
}
