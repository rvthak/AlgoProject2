#ifndef HASH_LSH
#define HASH_LSH

#include <string>
#include "List.h"
#include "Vector.h"
#include "bucket.h"
#include "hash_fun.h"
#include "shortedList.h"

#define DIVISION_SIZE 16

// Sinple Hash Table Struct that uses randomized Hash Functions
struct HashTable{
	Bucket *bucs;	// Array of Buckets
	unsigned size;	// The amount of Buckets
	G *g;			// The Hash function used on this Hash Table

	HashTable(int k, unsigned tableSize, unsigned v_size);
	~HashTable();

	int add(Vector *vec);

	void preview();
	double averageBucketSize();
};

// Multiple Hash Tables used for LSH
struct MultiHash{
	HashTable **array;	// Array containing the Hash Tables
	unsigned amount;	// The amount of Hash Tables

	MultiHash(int k, int L, unsigned tableSize, unsigned v_size);
	~MultiHash();

	void add(Vector *vec);
	void loadVectors(VectorArray *arr);
	void loadVectors(AssignmentArray *arr);
	void print();
	double averageBucketSize();

	ShortedList *kNN_lsh(Vector *query, unsigned k);
	List *range_search(Vector *query, double R);
};

#endif
