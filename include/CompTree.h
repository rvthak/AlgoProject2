#ifndef COMPLETE_TREE
#define COMPLETE_TREE

#include <vector>
#include "Vector.h"
#include "VCurve.h"

// A node of a complete tree
struct CompTreeNode{
	CompTreeNode *parent;		// The node's parent node
	CompTreeNode *left, *right;	// The node's children
	Vector *data;				// The node's data

	CompTreeNode(Vector *vec, CompTreeNode *parent);
	~CompTreeNode();

	bool isLeaf();
	void print();

	bool add(Vector *vec, unsigned cur_level, unsigned max_level);
	void grow(unsigned cur_level, unsigned max_level);
	VCurve *postOrderTraversal();
};

// A Complete Tree Struct
struct CompTree{
	CompTreeNode *root;	// The root node of the tree
	unsigned level;		// The current amount of levels the tree has
	unsigned nodes;		// The amount of nodes the tree has
	Centroid *cent;		// The Centroid used to create this Tree

	unsigned output_length;	// The requested size of the clusterMean() output vector

	// Create a complete tree by scattering the assigned vectors of the given Centroid
	CompTree(Centroid *cent);
	~CompTree();

	// Returns an approximation of the cluster's mean Vector
	std::vector<double> clusterMean();

	void print();
	bool isEmpty();

private:
	void add(Vector *vec);
	void grow(unsigned max_level);
};

#endif
