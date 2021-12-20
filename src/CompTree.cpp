#include "frechet.hpp"
#include "CompTree.h"
#include "point.hpp"
#include "VCurve.h"
#include "utils.h"

#include <iostream>
#include <cmath>

using namespace std;

//------------------------------------------------------------------------------------------------------------------

CompTreeNode::CompTreeNode(Vector *vec, CompTreeNode *parent){
	this->data   = vec;
	this->parent = parent;
	this->left   = nullptr;
	this->right  = nullptr;
}

CompTreeNode::~CompTreeNode(){
	if( this->left  != nullptr ){ delete (this->left ); }
	if( this->right != nullptr ){ delete (this->right); }
}

// Recursively replicate empty nodes until you reach the given level
void CompTreeNode::grow(unsigned cur_level, unsigned max_level){

	// When the goal-level is reached => return
	if(cur_level == max_level){ return; }

	// Else, create children Recursively
	this->left  = new CompTreeNode(nullptr, this);
	this->right = new CompTreeNode(nullptr, this);

	// Then grow them as well
	this->left ->grow(cur_level+1, max_level);
	this->right->grow(cur_level+1, max_level);
}

// Recursively add the given Vector to the final level of the tree (On a leaf)
// Returns 'true' when the Vector was added to a leaf
bool CompTreeNode::add(Vector *vec, unsigned cur_level, unsigned max_level){

	// If we reached the final level
	if( cur_level == max_level ){
		// If there is space on this node
		if( this->data == nullptr ){
			// Add it on this node and return success
			this->data = vec;
			return true;
		}
		else{
			// No space to store it on this node => return false
			return false;
		}
	}

	// We are not yet on the final level => 
	// Try to Add it on the kids
	if( (this->left ->add(vec, cur_level+1, max_level)) == true ){ return true; }
	if( (this->right->add(vec, cur_level+1, max_level)) == true ){ return true; }

	// If everything has failed to add the vector => return false
	return false;
}

bool CompTreeNode::isLeaf(){
	// Returns true if the node is a tree leaf
	return ( ((this->left) == nullptr) && ((this->right) == nullptr) );
}

// Recursive Post Order traversal - Used to calculate the new Cluster mean curve
// Based on the pseudocode provided ( 2.curves.pdf - Slide 33 )
VCurve *CompTreeNode::postOrderTraversal(){

	// If this node is a leaf => just return its Curve
	if( this->isLeaf() ){
		return new VCurve(&(this->data->vec));
	}

	VCurve *leftcur, *rightcur;
	
	// Since this node is not a leaf => It has at least a left child
	leftcur = this->left->postOrderTraversal();

	// If it also has a right child
	if( this->right != nullptr ){
		rightcur = this->right->postOrderTraversal();
	}

	// Finally, return the mean of the found curves
	VCurve *mean = meanCurve(leftcur, rightcur);
	delete leftcur; delete rightcur;
	return mean;
}

void CompTreeNode::print(){
	cout << " (p) CompTreeNode: "           << endl;
	cout << "     parent: " << this->parent << endl;
	cout << "     left:   " << this->left   << endl;
	cout << "     right:  " << this->right  << endl;
	cout << "     Vector: "; this->data->print(); cout << endl;
}

//------------------------------------------------------------------------------------------------------------------

CompTree::CompTree(Centroid *cent){

	// Init the Struct data members
	this->root  = nullptr;
	this->cent  = cent;
	this->level = 0;
	this->nodes = 0;
	this->output_length = 0;

	// Add the Vectors Assigned in the given Centroid in the tree 
	// We don't care about the order
	this->nodes = cent->assignments.size();
	cout << " (i) Running CompTree() for Cluster of size: " << this->nodes << endl;

	if( (this->nodes)==0 || (this->nodes)==1 ){
		cout << " (!) Warning: Not enough Vectors to get a mean curve from" << endl;
	}

	if( (this->nodes) >= 1 ){ this->output_length = cent->assignments[0]->size(); }

	// Create a tree of height log(vector_amount) with no Vectors stored
	// (log(vector_amount) height => enough leaves to store all the Vectors )
	this->level = ceil(log2(this->nodes));
	this->root->grow( 0, this->level );

	// For each Vector assigned on the Centroid
	for(unsigned i=0; i<(this->nodes); i++){
		// Add it to the tree
		this->add( (cent->assignments[i]) );
	}
}

CompTree::~CompTree(){
	// Just delete the root node, destructors will take care of the rest
	delete (this->root);
}

// Add the given Vector to the final level of the tree (On a leaf)
// Returns 'true' when the Vector was added to a leaf
void CompTree::add(Vector *vec){

	// Just add the vector to the root
	// It will recursively get added to a leaf
	this->root->add(vec, 0, this->level);
}

// Create and Grow the tree
void CompTree::grow(unsigned max_level){

	// This function can be called only if the tree is currently empty
	if( !(this->isEmpty()) ){
		cout << " (i) Warning: Attempted to call grow() with a non-empty tree." << endl;
		return;
	}

	// Create the root node
	this->root = new CompTreeNode(nullptr, nullptr);

	// Grow the root down to the requested level
	this->root->grow(0, max_level);
}

// Returns an approximation of the cluster's mean Vector
// Uses Post-Order-Traversal and means the vectors in pairs from the leafs
// to the root. (Like a reduce operation)
std::vector<double> CompTree::clusterMean(){

	if( this->nodes == 0 ){
		// No Vectors assigned => return the same old Centroid
		return this->cent->vec.vec;
	} 
	else if( this->nodes == 1 ){
		// Only one Vector assigned => It is the new Centroid
		return this->root->data->vec;
	}

	std::vector<double> mean_vec;

	// Else traverse the tree and calculate the approximate mean curve
	VCurve *mean_cur = this->root->postOrderTraversal();

	// Convert the curve to a Vector of the needed size
	mean_vec = mean_cur->vectorize_size(this->output_length);
	delete mean_cur; 

	return mean_vec;
}

// Prints the tree data members
void CompTree::print(){
	cout << " (p) CompTree: "               << endl;
	cout << "     root:    " << this->root  << endl;
	cout << "     level:   " << this->level << endl;
	cout << "     nodes:   " << this->nodes << endl;
	cout << "     cent:    " << this->cent  << endl;
	cout << "     outlen:  " << this->output_length << endl;
}

// Returns true if the tree is currently empty
bool CompTree::isEmpty(){ return ((this->root) == nullptr); }
