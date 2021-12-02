#ifndef SHORTED_LIST
#define SHORTED_LIST

#include "Vector.h"

// A node of the Shorted linked list
struct SL_Node{
	Vector *v;		// The stored Vector
	double dist;	// Its l2 distance to the target Vector
	SL_Node *next;	// A Pointer to the next list node
	SL_Node *prev;	// A Pointer to the previous list node

	SL_Node(Vector *v, double dist);
	void print();
};

// Used to store the top "max_len" closest neighbors to the target Vector
struct ShortedList{
	SL_Node *first;		// Pointer to the first list node (Smallest distance)
	SL_Node *last;		// Pointer to the last list node (longest stored distance)
	unsigned cur_len;	// Current length of the list
	unsigned max_len;	// Maximum length of the list

	ShortedList(unsigned max);
	~ShortedList();

	// Add the given Vector in the correct location while not storing more than "max_len" Vectors
	int add(Vector *v, double dist);
	bool exists(Vector *v, double dist);
	void print();

private:
	void delete_last();
};

#endif
