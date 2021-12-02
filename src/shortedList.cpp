#include "shortedList.h"
#include <iostream>

// Just store the data and init the "next" ptr
SL_Node::SL_Node(Vector *v, double dist){
	this->v = v;
	this->dist = dist;
	this->next = nullptr;
	this->prev = nullptr;
}

void SL_Node::print(){

	std::cout << " Id: " << this->v->id << ", Distance: " << this->dist << std::endl;
}

//------------------------------------------------------------------------------------------------------------------

// Create an empty Shorted List
ShortedList::ShortedList(unsigned max){
	this->first = nullptr;
	this->last = nullptr;
	this->cur_len = 0;
	this->max_len = max;
}

ShortedList::~ShortedList(){
	SL_Node *cur = this->first, *tmp;
	while( (tmp=cur) != nullptr ){
		cur = cur->next;
		delete tmp;
	}
}

// Insert the given Vector in the correct location in the list based on its given distance
// Ensure that each Vector exists only once in the list
int ShortedList::add(Vector *v, double dist){

	// If the list is empty just add it
	if( this->first == nullptr ){
		this->first = new SL_Node(v, dist);
		this->last = this->first;
		(this->cur_len)++;
		//std::cout << " Empty => New First " << std::endl;
		return 0;
	}

	// If the list is full => Check if the new Vector should replace any existing one or not
	if( this->cur_len == this->max_len ){

		// If the last item is smaller than the new one
		if( this->last->dist < dist ){
			// No point in storing the new Vector, it is not a part of the top 'k'
			//std::cout << " 1.Larger than last " << std::endl;
			return 1;
		}

		// If the Vector is already in the List, dont add it again
		//std::cout << " 1.Existence Check " << std::endl;
		if( this->exists(v, dist) ){ return 1; }

		// The new Vector is one of the top 'k' =>
		// Remove the largest Vector (the last one) to free up space
		this->delete_last();
		//std::cout << " 1.Deleted last " << std::endl;
	}
	else{ // The list is not full
		// If the Vector is already in the List, dont add it again
		if( this->exists(v, dist) ){ return 1; }
		//std::cout << " 2.Existence Check " << std::endl;
	}

	// Add the New Vector in the correct location

	// Find the correct location
	SL_Node *cur = this->first, *prev, *tmp;
	while( (cur!=nullptr) && (cur->dist < dist) ){
		prev = cur;
		cur = cur->next;
	}

	// Add it and link accordingly
	if( cur == this->first ){	// New first item

		// In case we have a Shorted list with max_size == 1
		if( this->first == nullptr ){
			this->first = new SL_Node(v, dist);
			this->last = this->first;
		}
		else{
			tmp = this->first; 
			this->first = new SL_Node(v, dist);
			this->first->next = tmp;
			tmp->prev = this->first;
		}
		//std::cout << " New First " << std::endl;
	}
	else if( cur == nullptr ){	// New last item
		this->last = new SL_Node(v, dist);
		this->last->prev = prev;
		prev->next = this->last;
		//std::cout << " New Last " << std::endl;
	}
	else{	// New item in between
		prev->next = new SL_Node(v, dist);
		prev->next->prev = prev;
		prev->next->next = cur;
		cur->prev = prev->next;
		//std::cout << " New between " << std::endl;
	}
	(this->cur_len)++;

	return 0;
}

// Check if the given Vector exists in the Shorted list
// Take advantage of the fact that the list is shorted to end the search early
bool ShortedList::exists(Vector *v, double dist){
	// Start from the first node and check every node
	SL_Node *cur = this->first;
	while( cur != nullptr ){

		// If we passed the ids distance => it does not exist for sure
		if( cur->dist > dist ){ return false; }

		if( cur->v->id == v->id){ return true; }
		cur = cur->next;
	}
	return false;
}

// Remove the current last item from the list
void ShortedList::delete_last(){
	(this->cur_len)--;

	if(this->cur_len == 0){
		delete this->first;
		this->first = nullptr;
		this->last = nullptr;
	}
	else{
		SL_Node *tmp;
		tmp = this->last->prev;
		tmp->next = nullptr;
		delete this->last;
		this->last = tmp;
	}
}

// Print the shorted list
void ShortedList::print(){
	std::cout << std::endl << " (i) Printing Shorted List: ";
	std::cout << "  Cur Length: " << this-> cur_len << ", Max Length:" << this->max_len << std::endl;

	SL_Node *cur = this->first;

	while( cur != nullptr ){
		cur->print();
		cur = cur->next;
	} std::cout << std::endl;
}
