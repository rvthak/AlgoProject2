#ifndef LIST
#define LIST

#include "Vector.h"

// List Node Struct
struct List_node{
	List_node *next;	// The next node in the linked list
	Vector *data;		// A pointer to the Vector data of this node

	List_node(Vector *v);
	~List_node();
};

// Linked List Stucture Containing Vectors
struct List{
	List_node *first;	// Pointer to the first bucket item
	unsigned size;		// The amount of items stored in the bucket

	List();
	~List();

	int add(Vector *v);
	bool exists(Vector *v);
	void print();
};

#endif
