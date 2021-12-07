#include "utils.h"
#include "Vector.h"
#include "shortedList.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

Vector::Vector(){ this->id = 0; this->centroid = nullptr; }

// Prints all the data stored in a Vector
void Vector::print(){
	cout << " Id: " << this->id << ", Name: " << this->name << endl << "   > ";
	for (double val: this->vec){
		cout << val << ' ';
	} cout << endl;
}

// Calculate the norm between "this" and p
double Vector::l2(Vector *p){
	double tmp;
	double sum=0;

	for(long unsigned i=0; i<(p->vec).size(); i++){
		tmp = (this->vec)[i] - (p->vec)[i]; // No need for abs() since we always square the difference
		sum += tmp * tmp;
	}
	return sqrt(sum);
}

double Vector::discrete_frechet_distance(Vector *p)
{
	// cout << "Welcome to discrete frechet distance!" << endl;

    Vector* q = this;

    unsigned length_p = (p->vec.size()) - 1;
    unsigned length_q = (q->vec.size()) - 1;

	// cout << "length_p : " << length_p << endl;
	// cout << "length_q : " << length_p << endl; 

	double** c = new double*[length_q];
    double** distances = new double*[length_q];

    for (unsigned i = 0; i < length_q; i++)
    {
		c[i] = new double[length_p];
        distances[i] = new double[length_p];
    }

	// cout << "Allocated 2D array!" << endl;

    for (unsigned i = 0; i < length_p; i++)
    {
        for (unsigned j = 0; j < length_q; j++)
        {
			double x_i = i;
			double y_i = p->vec[i];

			double x_j = j;
			double y_j = p->vec[j];

            distances[i][j] = distance_between_points(x_i, y_i, x_j, y_j);

			// cout << "distances[" << i << "][" << j << "] : " << distances[i][j] << endl;
        }
    }

	// cout << "Initialized 2D array!" << endl;


	for (unsigned i = 0; i < length_p; i++)
    {
        for (unsigned j = 0; j < length_q; j++)
        {
			if (i == 0 && j == 0) 
				c[i][j] = distances[i][j];
			else if (i == 0 && j > 0)
				c[i][j] = max(c[i][j - 1], distances[i][j]);
			else if (i > 0 && j == 0)
				c[i][j] = max(c[i - 1][j], distances[i][j]);
			else
				c[i][j] = max(min(min(c[i - 1][j], c[i - 1][j - 1]), c[i][j - 1]), distances[i][j]);

			// cout << "c[" << i << "][" << j << "] : " << c[i][j] << endl;
		}
	}

	// cout << "Finished loop!" << endl;

	double distance = c[length_p - 1][length_q - 2];

	// cout << "Discrete Frechet Distance : " << distance << endl;

	// if (c[length_p][length_q] == NULL)
	// 	cout << "NUUUUUULLLLLLLLLL!!!!" << endl;

	

    return distance;

	// double distance = c[length_p][length_q];
    
	// int p1 = p->vec[1];
    // int q1 = q->vec[1];

    // distances[1][1] =  abs(p1 - q1);

	// cout << "Going into recursion!" << endl;

    // double distance = this->dfd_calculation(distances, length_p, length_q, p, q);

	// double distance = 0;

	// cout << "Discrete Frechet Distance : " << distance << endl;

}


Vector* Vector::filter_vector(unsigned e)
{
    Vector* filtered_vector = new Vector();
    filtered_vector->id = this->id;

    unsigned current_vector_length = this->vec.size();

    for (unsigned i = 0; i < current_vector_length; i++)
    {
        unsigned index_a = i;
        unsigned index_b = i + 1;
        unsigned index_c = i + 2;

        if (index_a > current_vector_length && index_b > current_vector_length && index_c > current_vector_length)
            return filtered_vector;

        int a = this->vec[index_a];
        int b = this->vec[index_b];
        int c = this->vec[index_c];

        if (abs(a - b) <= e && abs(b - c) <= e)
        {
            filtered_vector->vec.push_back(a);
            filtered_vector->vec.push_back(c);
        }
        else
        {
            filtered_vector->vec.push_back(a);
            filtered_vector->vec.push_back(b);
            filtered_vector->vec.push_back(c);
        }
    }

    return filtered_vector;
}

//------------------------------------------------------------------------------------------------------------------

VectorArray::VectorArray(unsigned size){
	this->size = size;

	// Allocate memory to store the file records
	this->array = new Vector[this->size];
	if( this->array == nullptr ){
		cout << "\033[31;1m (!) Fatal Error:\033[0m Memory <<  Failed to allocate memory for file records." << endl;
		exit(1);
	}
}

// Create a VectorArray containing the given file contents
VectorArray::VectorArray(string filename){

	// Get the total amount of records in the target file
	this->size = getFileLines(filename);
	if( this->size == 0 ){
		cout << "\033[31;1m (!) Fatal Error:\033[0m Input Parsing : " << filename << " : File empty." << endl;
		exit(1);
	}

	// Allocate memory to store the file records
	this->array = new Vector[this->size];
	if( this->array == nullptr ){
		cout << "\033[31;1m (!) Fatal Error:\033[0m Memory  : " << filename << " : Failed to allocate memory for file records." << endl;
		exit(1);
	}

	// Parse the file and store the records
	this->parse_input(filename);
	//cout << " (i) Created VectorArray containing " << this->size << " vectors." << endl;
}

// Free a VectorArray
VectorArray::~VectorArray(){ delete [] this->array; }

// Add a vector in the given "index" of a VectorArray
int VectorArray::add_vector(unsigned index, int id, string name, vector<double> data){
	
	if( this->size < index ){ return 1; }

	this->array[index].id = id;
	this->array[index].vec = data;
	this->array[index].name = name;

	return 0;
}

// Prints all the Vectors Stored in a VectorArray
void VectorArray::print(){
	for(unsigned i=0; i<(this->size); i++){
		(this->array)[i].print();
		cout << endl;
	}
}

// Parse the given file and load its records in the VectorArray
void VectorArray::parse_input(string filename){

	// Amount of Integers per Vector (Does not count the Vector id)
	unsigned vec_length = getFileLineLength(filename)-1;
	//cout << " Vector Length: " << vec_length << endl;

	// Open the file as an ifstream
	ifstream file(filename);
	string line, name;

	unsigned vecs_loaded=0; // Counts the already parsed vectors
	double val;
	vector<double> tmp_vec;

	// For each Vector in the file (== for each line):
	while( getline(file, line) ){

		// Convert the line into a stream for easier parsing
		istringstream line_stream(line);

		// Get the Vector id name
		if( !(line_stream >> name) ){
			cout << "\033[31;1m (!) Fatal Error:\033[0m Input Parsing : " << filename << " : line no " << vecs_loaded << " : Failed to parse Id name." << endl;
			exit(1);
		}

		// Create a Vector containing all the existing numbers
		while( line_stream >> val ){
			tmp_vec.push_back(val); 
		}

		// If you get an illegal Vector, terminate
		if( tmp_vec.size() != vec_length ){
			cout << " Read Vector Length: " << tmp_vec.size() << endl;
			cout << "\033[31;1m (!) Fatal Error:\033[0m Input Parsing : " << filename << " : line no " << vecs_loaded << " : Unsupported Vector Length." << endl;
			exit(1);
		}//cout << " Parsed Vector: Id:" << id << ", Length:" << tmp_vec.size() << endl;

		// Add the new Vector to the VectorArray Storage Array
		this->add_vector(vecs_loaded, vecs_loaded, name, tmp_vec);

		// Clear the vector contents and reset the id to "Empty", to read the next Vector
		tmp_vec.clear();

		// This line's Vector was loaded successfully
		vecs_loaded++;
	}
}

// Naive search for the k Nearest Neighbors of the given query Vector
void *VectorArray::kNN_naive(Vector *query, unsigned k){

	// The naive approach to solving the k Nearest Neighbors problem is
	// to just check all the distances and keep the shortest ones

	// We just add all the existing Vectors and their distances into a Shorted List
	// and in the end we have the k nearest Vectors stored in the list
	ShortedList *list = new ShortedList(k);
	for(unsigned i=0; i<(this->size); i++){
		list->add( &((this->array)[i]), query->l2( &((this->array)[i]) ) );
	}
	return list;
} // (!) Remember to free the returned list afterwards

//------------------------------------------------------------------------------------------------------------------

// Prints all the data stored in a Centroid
void Centroid::print(){
	for (double val: this->vec.vec){
		cout << val << ' ';
	} cout << endl;
}

// Calculate the norm between "this" and p
double Centroid::l2(Vector *p){
	double tmp;
	double sum=0;

	for(long unsigned i=0; i<(p->vec).size(); i++){
		tmp = (this->vec.vec)[i] - (p->vec)[i];
		sum += tmp * tmp;
	}
	return sqrt(sum);
}

// Copies the values of the given Vector to the Centroid
void Centroid::copy_Vec(Vector *p){
	// Clear the current contents
	(this->vec.vec).clear();

	// Copy the vector over
	this->vec.vec = p->vec;
}

// Assign the given vector to this Centroid
void Centroid::assign(Vector *p){
	(this->assignments).push_back(p);
	(this->cluster_size)++;
	p->centroid = this;
}

//------------------------------------------------------------------------------------------------------------------

CentroidArray::CentroidArray(unsigned size){
	this->array = new Centroid[size];
	this->size = size;
	this->change = true;
}

CentroidArray::~CentroidArray(){ delete [] this->array; }

// Initialize the Centroids completely randomly (May lead to bad initial state => bad clustering)
void CentroidArray::initialize_random(void *ass_vecs){
	// A vector containing the indexes of the already assigned Initial Centroid-Vectors
	std::vector<int> assigned_indexes;
	unsigned random_index;
	srand(time(NULL));

	// Get a random Vector for each Centroid
	for(unsigned i=0; i<(this->size); i++){

		do{
			// Get a random Vector from the AssignmentArray
			random_index = rand()%( ((AssignmentArray *)ass_vecs)->size );
		// Ensure that it hasn't already been selected as a Centroid
		}while( find(assigned_indexes.begin(), assigned_indexes.end(), random_index) != assigned_indexes.end() );

		// Assign the Centroid
		(this->array)[i].copy_Vec( &((((AssignmentArray *)ass_vecs)->array)[random_index]) );

		// Add the Assigned Vector Index to a vector to avoid douplicate assignments
		assigned_indexes.push_back(random_index);
	}
}

// Use a probabilistic function to attempt to get good initial values for the Centroids
void CentroidArray::initialize_plus_plus(void *ass_vecs){
	unsigned selected_count=1, total=this->size;
	int max_index;
	std::vector<unsigned>   selected_indexes;
	double prob, max_prob;
	Vector *v;
	srand(time(NULL));

	// Choose a random centroid uniformly
	selected_indexes.push_back( rand() % (((AssignmentArray *)ass_vecs)->size) );
	(this->array)[0].copy_Vec( &((((AssignmentArray *)ass_vecs)->array)[ selected_indexes[0] ]) );

	// While we need to select more Centroids
	while( selected_count < total ){
		max_prob=-1; max_index=-1;

		// For each non-Centroid Vector
		for(unsigned i=0; i<( ((AssignmentArray *)ass_vecs)->size ); i++){
			if( !index_exists(i, &selected_indexes) ){
				v = &((((AssignmentArray *)ass_vecs)->array)[i]);

				// Get the probability of this Vector being a good next Centroid based on the already selected Centroids
				prob = this->get_prob(v, selected_count);

				// Store the Vector with the highest probability
				if( prob > max_prob ){
					max_prob = prob;
					max_index = i;
				}
			}
		}
		// Assign the Vector with the highest probability as the next Centroid
		selected_indexes.push_back( max_index );
		(this->array)[ selected_count ].copy_Vec( &((((AssignmentArray *)ass_vecs)->array)[ max_index ]) );
		selected_count++;
	}

	// cout << " Selected centroids: " << endl;
	// for (int i = 0; i < selected_indexes.size(); ++i)
	// {
	// 	cout << selected_indexes[i] << ", ";
	// } cout << endl;
}

// For the given Vector: calculate its probability of being a good choice for a Centroid 
// based on the formula on slide 45
double CentroidArray::get_prob(Vector *v, unsigned selected){
	double dist, min_dist=-1, dist_sum=0;

	// For each already selected Centroid
	for(unsigned i=0; i<selected; i++){

		// Calc the distance between the given Vector and this Centroid
		dist = (this->array)[i].l2(v);

		// Store the min distance
		if( positive_less_than(dist, min_dist) ){
			min_dist = dist;
		}

		// Calc the squared sum
		dist_sum += dist*dist;
	}
	return (min_dist*min_dist)/dist_sum;
}

// Prints all the Centroids Stored
void CentroidArray::print(){
	for(unsigned i=0; i<(this->size); i++){
		(this->array)[i].print();
		cout << endl;
	}
}

// Resets all the assignments
void CentroidArray::reset_clusters(){
	// Reset all the Cluster Assignments
	for(unsigned i=0; i<(this->size); i++){
		(this->array)[i].cluster_size = 0;
		(this->array)[i].assignments.clear();
	}
}

// return 'true' if one or more assignments changed
bool CentroidArray::changed(){ return this->change; }

// Returns the index of the given Centroid-Vector or -1 if it doesnt exist
int CentroidArray::get_index(Vector *vec){
	for(unsigned i=0; i<(this->size); i++){
		if( (this->array)[i].vec.vec == vec->vec ){return i;}
	}
	return -1;
}

//------------------------------------------------------------------------------------------------------------------

// Create a AssignmentArray containing the given file contents
AssignmentArray::AssignmentArray(std::string filename){

	// Get the total amount of records in the target file
	this->size = getFileLines(filename);
	if( this->size == 0 ){
		cout << "\033[31;1m (!) Fatal Error:\033[0m Input Parsing : " << filename << " : File empty." << endl;
		exit(1);
	}

	// Allocate memory to store the file records
	this->array = new Vector[this->size];
	if( this->array == nullptr ){
		cout << "\033[31;1m (!) Fatal Error:\033[0m Memory  : " << filename << " : Failed to allocate memory for file records." << endl;
		exit(1);
	}

	// Parse the file and store the records
	this->parse_input(filename);

	// Init the Centroid Pointers to NULL 
	for(unsigned i=0; i<(this->size); i++){
		(this->centroid).insert({ (this->array)[i].id, nullptr });
		(this->dist).insert({ (this->array)[i].id, 0 });
	}
}

AssignmentArray::~AssignmentArray(){ delete [] this->array; }

// Store the Centroid and distance assigned to the Vector with the given id
void AssignmentArray::assign(unsigned id, Centroid *centroid, double dist){
	(this->centroid)[id] = centroid;
	(this->dist)[id] = dist;
}

// Prints all the Vectors Stored in the AssignmentArray
void AssignmentArray::print(){
	for(unsigned i=0; i<(this->size); i++){
		(this->array)[i].print();
		cout << " Centroid: " << endl;
		if( (this->centroid)[i] == nullptr ){ cout << " NULL " << endl; }
		else{ (this->centroid)[i]->print(); }
		cout << " Distance: " << (this->dist)[i] << endl << endl;
	}
}

// Reset all the assignments made
void AssignmentArray::reset_clusters(){

	// Reset all the Vector pointer assignemts
	for(unsigned i=0; i<(this->size); i++){
		(this->array)[i].centroid = nullptr;
	}

	// Reset all the map Assignments
	for(unsigned i=0; i<(this->size); i++){
		(this->centroid)[i] = nullptr;
		(this->dist)[i] = 0;
	}
}

// Add a vector in the given "index" of a AssignmentArray
int AssignmentArray::add_vector(unsigned index, int id, string name, vector<double> data){
	if( this->size < index ){ return 1; }
	this->array[index].id = id;
	this->array[index].vec = data;
	this->array[index].name = name;
	return 0;
}

// Parse the given file and load its records in the AssignmentArray
void AssignmentArray::parse_input(string filename){

	// Amount of Integers per Vector (Does not count the Vector id)
	unsigned vec_length = getFileLineLength(filename)-1;

	// Open the file as an ifstream
	ifstream file(filename);
	string line, name;

	unsigned vecs_loaded=0; // Counts the already parsed vectors
	double val;
	vector<double> tmp_vec;

	// For each Vector in the file (== for each line):
	while( getline(file, line) ){

		// Convert the line into a stream for easier parsing
		istringstream line_stream(line);

		// Get the Vector id name
		if( !(line_stream >> name) ){
			cout << "\033[31;1m (!) Fatal Error:\033[0m Input Parsing : " << filename << " : line no " << vecs_loaded << " : Failed to parse Id name." << endl;
			exit(1);
		}

		// Create a Vector containing all the existing numbers
		while( line_stream >> val ){
			tmp_vec.push_back(val); 
		}

		// If you get an illegal Vector, terminate
		if( tmp_vec.size() != vec_length ){
			cout << "\033[31;1m (!) Fatal Error:\033[0m Input Parsing : " << filename << " : line no " << vecs_loaded << " : Illegal Vector." << endl;
			exit(1);
		}//cout << " Parsed Vector: Id:" << id << ", Length:" << tmp_vec.size() << endl;

		// Add the new Vector to the VectorArray Storage Array
		this->add_vector(vecs_loaded, vecs_loaded, name, tmp_vec);

		// Clear the vector contents and reset the id to "Empty", to read the next Vector
		tmp_vec.clear();

		// This line's Vector was loaded successfully
		vecs_loaded++;
	}
}

//------------------------------------------------------------------------------------------------------------------

// < Update State > : Update Centroids

// Update the Centroid Values by calculating vector averages
void AssignmentArray::update_centroids(CentroidArray *cent){
	unsigned vec_size = (cent->array)[0].vec.vec.size();

	cent->change = false;

	// For each Centroid (== for each Cluster)
	for(unsigned i=0; i<(cent->size); i++){

		// If no Vectors are assigned to this Centroid => don't move it
		if( (cent->array)[i].assignments.size() == 0 ){ continue; }

		std::vector<double> mean(vec_size, 0);
		std::vector<double> prev = (cent->array)[i].vec.vec;

		// Sum all the vectors that belong to this cluster
		for(unsigned j=0; j<( (cent->array)[i].assignments.size() ); j++){
			mean = sum_vectors(&mean, &(((cent->array)[i].assignments[j])->vec) );
		}
		// And assign the new Cluster Centroid
		(cent->array)[i].vec.vec = div_vector(&mean, (cent->array)[i].assignments.size() );

		// If any centroid changes => update "changed" flag
		if( !(prev == (cent->array)[i].vec.vec) ){ cent->change = true; }
		//cout << " Updated: " << i << endl;
		//(cent->array)[i].vec.print();
	}
}
