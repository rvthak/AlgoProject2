#include "List.h"
#include <iostream>

List_node::List_node(Vector *v){
	this->data = v;
	this->next = nullptr;
}

List_node::~List_node(){ }

//------------------------------------------------------------------------------------------------------------------

// Create an Empty Linked list
List::List(){
	this->first = nullptr;
	this->size = 0;
}

// Just delete all the elements of the list (if any) 
List::~List(){
	//std::cout << " FIRST: " << this->first << std::endl;
	List_node *cur = this->first, *tmp;
	while( (tmp=cur) != nullptr ){
		cur = cur->next;
		delete tmp;
	}
}

// Store the Vector Pointer in the List ( Insert at Start : O(1) )
// Ensure that each Vector exists only once in the list
int List::add(Vector *v){

	// If the Vector is already in the List, dont add it again
	if( this->exists(v) ){ return 1; } 

	List_node *tmp = this->first;
	this->first = new List_node(v);
	if( (this->first) == nullptr ){ return 1; }
	this->first->next = tmp;
	return 0;
}

// Search for the given Vector in the List's Linked List
bool List::exists(Vector *v){
	List_node *cur = this->first;
	while( cur != nullptr ){
		if( cur->data->id == v->id ){ return true; }
		cur = cur->next;
	}
	return false;
}

// Print the contents of the List
void List::print(){
	std::cout << std::endl << " (i) Printing List: " << std::endl;
	
	List_node *cur = this->first;
	while( cur != nullptr ){
		cur->data->print();
		cur = cur->next;
	} std::cout << std::endl;
}
