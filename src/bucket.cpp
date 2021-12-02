#include "bucket.h"
#include <iostream>

Bucket_node::Bucket_node(Vector *v, unsigned ID){
	this->data = v;
	this->ID = ID;
	this->next = nullptr;
}

Bucket_node::~Bucket_node(){ }

//------------------------------------------------------------------------------------------------------------------

// Create an Empty Linked list
Bucket::Bucket(){
	this->first = nullptr;
	this->size = 0;
}

// Just delete all the elements of the list (if any) 
Bucket::~Bucket(){
	//std::cout << " FIRST: " << this->first << std::endl;
	Bucket_node *cur = this->first, *tmp;
	while( (tmp=cur) != nullptr ){
		cur = cur->next;
		delete tmp;
	}
}

// Store the Vector Pointer in the Bucket (insert at Start)
int Bucket::add(Vector *v, unsigned ID){
	Bucket_node *tmp = this->first;
	this->first = new Bucket_node(v, ID);
	if( (this->first) == nullptr ){ return 1; }
	this->first->next = tmp;
	(this->size)++;
	return 0;
}

// Search for the given Vector in the Bucket's Linked List
bool Bucket::search(Vector *v){
	Bucket_node *cur = this->first;
	while( cur != nullptr ){
		if( cur->data->id == v->id ){ return true; }
		cur = cur->next;
	}
	return false;
}

// Print the contents of the Bucket
void Bucket::print(){
	std::cout << std::endl << " (i) Printing Bucket List: " << std::endl;
	
	Bucket_node *cur = this->first;
	while( cur != nullptr ){
		cur->data->print();
		cur = cur->next;
	} std::cout << std::endl;
}
