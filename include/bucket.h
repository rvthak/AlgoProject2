#ifndef BUCKET
#define BUCKET

#include "Vector.h"

// The Bucket linked list node
struct Bucket_node{
	Bucket_node *next;	// The next node in the linked list
	Vector *data;		// A pointer to the Vector data of this node
	unsigned ID;		// The ID of this Vector

	Bucket_node(Vector *v, unsigned ID);
	~Bucket_node();
};

// Bucket of Hash Table - A simple liked list (This Struct is the head of the linked list)
struct Bucket{
	Bucket_node *first;	// Pointer to the first bucket item
	unsigned size;		// The amount of items stored in the bucket

	Bucket();
	~Bucket();

	int add(Vector *v, unsigned ID);
	bool search(Vector *v);
	void print();
};

#endif
