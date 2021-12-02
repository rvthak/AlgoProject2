#ifndef HASH_CUBE
#define HASH_CUBE

#include "bucket.h"
#include "Vector.h"
#include "hash_fun.h"
#include "shortedList.h"
#include "List.h"
#include <vector>

using namespace std;

#define DIVISION_SIZE_CUBE 16

// Sinple Hash Table Struct that uses randomized Hash Functions
struct Hypercube{
	Bucket *bucs;	             // Array of Buckets
	ShortedList *shorted_list; // The sorted list that will help with the search
	unsigned size;	           // The amount of Buckets
	unsigned probes;           // Max amount of buckets to check
	unsigned M;                // Max amount of vectors to check
	unsigned k;                // k nearest-neighbors to check
	unsigned probes_searched;
	unsigned vectors_searched;
	vector<int> visited_bucket_keys;
	F *f;			                 // The Hash function used on this Hash Table

	Hypercube(unsigned k, unsigned tableSize, unsigned v_size);
	~Hypercube();

	int add(Vector *vec);
	void loadVectors(VectorArray *arr);
	void loadVectors(AssignmentArray *arr);
	void set_search_limits(unsigned probes, unsigned M, unsigned k);
	int project_query_vector(Vector *query_vector);
	void iterate_bucket(Bucket* bucket, Vector* query_vector);
	int get_next_bucket_key(int last_bucket_key);
	void search_hypercube(Vector *query);
	ShortedList* k_nearest_neighbors_search(unsigned k);
	List* range_search(double range);
};

#endif
