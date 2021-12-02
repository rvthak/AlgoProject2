#include "hash_cube.h"
#include "shortedList.h"
#include "List.h"
#include "utils.h"
#include "Vector.h"
#include "bucket.h"
#include <iostream>
#include <vector>
#include <algorithm>

#define MAX_32_INT 2147483647

using namespace std;

Hypercube::Hypercube(unsigned k, unsigned tableSize, unsigned v_size){

	// Allocate the Bucket Array
	this->size = tableSize;
	this->bucs = new Bucket[tableSize];
	this->shorted_list = new ShortedList(MAX_32_INT);

	if(this->bucs == nullptr)
	{
		cout << "\033[31;1m (!) Fatal Error:\033[0m Memory : Failed to allocate memory for Bucket." << endl;
		exit(1);
	}

	if(this->shorted_list == nullptr)
	{
		cout << "\033[31;1m (!) Fatal Error:\033[0m Memory : Failed to allocate memory for Shorted List." << endl;
		exit(1);
	}

	// Create the randomized hash function
	this->f = new F(k, v_size, tableSize);

	if(this->f == nullptr)
	{
		cout << "\033[31;1m (!) Fatal Error:\033[0m Memory : Failed to create F hash function." << endl;
		exit(1);
	}
}

Hypercube::~Hypercube(){
	delete [] this->bucs;
	delete this->f;
	delete this->shorted_list;
}


// Add the given Vector to "this" Hash Table
// We get the bucket key from the F hash struct
int Hypercube::add(Vector *vec)
{
	unsigned key = this->f->hash(vec);
	return (this->bucs)[key].add(vec, 0); // ID == 0 | We don't use ID for Cube
}

void Hypercube::loadVectors(VectorArray *arr)
{
	for(unsigned i = 0; i < (arr->size); i++)
	{
		this->add(&((arr->array)[i]));
	}
}


void Hypercube::loadVectors(AssignmentArray *arr)
{
	for(unsigned i = 0; i < (arr->size); i++)
	{
		this->add(&((arr->array)[i]));
	}
}


void Hypercube::set_search_limits(unsigned probes, unsigned M, unsigned k)
{
	this->probes = probes;
	this->M = M;
	this->k = k;
}

int Hypercube::project_query_vector(Vector* query_vector)
{
	unsigned projection_key = this->f->hash(query_vector);
	return projection_key;
}

int Hypercube::get_next_bucket_key(int last_bucket_key)
{
	vector<int> hamming_distance_limits { 5, 4, 3, 2, 1 }; 

	for (unsigned i = 0; i < hamming_distance_limits.size(); i++)
	{
		// First check for hamming distance 1, then 2 and so on
		int current_hamming_distance_limit = hamming_distance_limits.back();
		hamming_distance_limits.pop_back();

		for (unsigned j = 0; j < this->size; j++)
		{
			// Check if we have already visited that bucket
			if (find(this->visited_bucket_keys.begin(), this->visited_bucket_keys.end(), j) != this->visited_bucket_keys.end())
				continue;

			int candidate_key = j;
			int hamming_dist = hamming_distance(candidate_key, last_bucket_key);

			if (hamming_dist == current_hamming_distance_limit)
				return candidate_key;
			else
				continue;
		}
	}

	return 0;
}

void Hypercube::iterate_bucket(Bucket* bucket, Vector* query_vector)
{
	Bucket_node *current_bucket_node = bucket->first;

	// TODO : Uncomment line bellow
	// if (current_bucket_node == nullptr) cout << "nullptr!!" << endl;

	while (current_bucket_node != nullptr)
	{
		// Get each vector of the bucket & the L2 distance to the query vector
		Vector* vector = current_bucket_node->data;
		double distance = query_vector->l2(vector);
		this->shorted_list->add(vector, distance);
		current_bucket_node = current_bucket_node->next;
		this->vectors_searched++;
	}
}

void Hypercube::search_hypercube(Vector *query)
{
	// Re-Initialize the shorted list if it already exists
	if (this->shorted_list != nullptr)
	{
		delete this->shorted_list;
		this->shorted_list = new ShortedList(MAX_32_INT);
	}

	this->probes_searched = 0;
	this->vectors_searched = 0;
	this->visited_bucket_keys = vector<int>();

	unsigned max_probes = this->probes;
	unsigned max_vectors = this->M;
	int last_key = 0;

	// Look int the first projected bucket, the next buckets will be looked according to hamming distance
	int projection_key = this->project_query_vector(query);
	Bucket* projection_bucket = &(this->bucs)[projection_key];

	this->iterate_bucket(projection_bucket, query);
	last_key = projection_key;
	visited_bucket_keys.push_back(last_key);

	// Search through a bucket, get the next one and do it all over again
	while ((this->probes_searched <= max_probes) && (this->vectors_searched <= max_vectors))
	{
		int current_bucket_key = get_next_bucket_key(last_key);
		Bucket* projection_bucket = &(this->bucs)[current_bucket_key];

		this->iterate_bucket(projection_bucket, query);
		this->probes_searched++;
		last_key = current_bucket_key;
		visited_bucket_keys.push_back(last_key);
	}
}

ShortedList* Hypercube::k_nearest_neighbors_search(unsigned k)
{
	// From the shorted list, get the k first elemnts

	unsigned counter = 0;
	ShortedList* final_list = new ShortedList(k);
	SL_Node* current_list_node = this->shorted_list->first;

	while ((current_list_node != nullptr) && (counter <= k))
	{
		Vector* vector = current_list_node->v;
		double distance = current_list_node->dist;
		final_list->add(vector, distance);
		current_list_node = current_list_node->next;
	}

	return final_list;
}

List* Hypercube::range_search(double range)
{
	// From the shorted list get the elements that have
	// a distance in the range provided

	List* final_list = new List();
	SL_Node* current_list_node = this->shorted_list->first;

	while (current_list_node != nullptr)
	{
		Vector* vector = current_list_node->v;
		double distance = current_list_node->dist;

		if (distance <= range)
			final_list->add(vector);

		current_list_node = current_list_node->next;
	}

	return final_list;
}
