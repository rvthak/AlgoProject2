#ifndef VECTOR
#define VECTOR

#include <map>
#include <vector>
#include <string>
#include <iterator>

#include "curve.hpp"

// Vector Representation Struct
struct Vector{
	unsigned id;			// The Vector's Id
	std::vector<double> vec;	// The Vector Itself
	void *centroid;			// A pointer to the assigned Centroid when clustering (if any)
	std::string name;		// The id-name of the Vector

	Vector();
	Vector(std::vector<double> v);
	Vector(std::vector<double> *v);

	void print();				// Print the Vector Contents
	unsigned size();			// Returns the size of the Vector
	Curve* create_Curve();		// Create a 2D Curve representation of the given Vector
	Curve* create_Curve_1D();	// Create a 1D Curve representation of the given Vector

	double l2(Vector *p);		// Returns the l2 norm between the two vectors
	double discrete_frechet_distance(Vector *p);		// Returns the discrete frechet distance between the two vectors
	double discrete_frechet_distance_fred(Vector *p);	// Discrete Frechet but using Fred Lib (Used for testing only)
	double continuous_frechet_distance(Vector *p);		// Returns the continuous frechet distance between the two vectors
};

// Define a Vector member function type for the Distance functions in Vector
typedef double (Vector::*VecDist)(Vector *);

// Array Struct Used to parse and store Vectors
struct VectorArray{
	unsigned size;	// The size of the Array == The amount of Vectors
	Vector *array;	// The Vector Storage Array itself

	VectorArray(unsigned size);

	VectorArray(std::string filename);
	~VectorArray();

	void *kNN_naive(Vector *query, unsigned k, unsigned mode);
	void print();	// Print all the Vectors of the Array

	int add_vector(unsigned index, int id, std::string name, std::vector<double> data);

private:
	void parse_input(std::string filename);
};

//------------------------------------------------------------------------------------------------------------------

// Centroid Representation Struct
struct Centroid{
	Vector vec;							// The Centroid Itself
	unsigned cluster_size;				// The amount of Vectors Assigned on this Centroid's Cluster
	std::vector<Vector *> assignments; 	// A vector containing all the Vectors currently assigned on this cluster

	void print();				// Print the Vector Contents
	double l2(Vector *p);		// Returns the l2 norm between the two vectors
	double discrete_frechet_distance(Vector *p);		// Returns the discrete frechet distance between the two vectors
	unsigned size();

	void copy_Vec(Vector *p);	// Copies the values of the given Vector to the Centroid
	void assign(Vector *p);		// Assign the given vector to this Centroid
};

// Define a Centroid member function type for the Distance functions in Centroids
typedef double (Centroid::*CenDist)(Vector *);

// Array Struct Used to store Centroids
struct CentroidArray{
	unsigned size;		// The size of the Array == The amount of Centroids
	Centroid *array;	// The Centroid Storage Array itself
	bool    change;		// Boolean that indicates if one or more Centroid changes were made

	CentroidArray(unsigned size);
	~CentroidArray();

	void initialize_random(void *ass_vecs);
	void initialize_plus_plus(void *ass_vecs);

	void reset_clusters();
	int  get_index(Vector *vec);
	bool changed();
	void print();
private:
	double get_prob(Vector *v, unsigned selected);
};

//------------------------------------------------------------------------------------------------------------------

// Array Struct Used to store Vector-Centroid Assignments
struct AssignmentArray{
	unsigned size;			// The size of the Array == The amount of Vectors
	Vector   *array;		// The Vector Storage Array itself

	std::map<unsigned, Centroid *> centroid;	// Foreach Vector id -> Assigned Centroid
	std::map<unsigned, double> dist;			// Foreach Vector id -> Distance to Assigned Centroid

	AssignmentArray(std::string filename);
	~AssignmentArray();

	void update_centroids(CentroidArray *cent);
	void update_curve_centroids(CentroidArray *cent);

	void reset_clusters();
	void assign(unsigned id, Centroid *centroid, double dist);
	void print();	// Print all the Vector-Centroid-distance pairs

	int add_vector(unsigned index, int id, std::string name, std::vector<double> data);

private:
	void parse_input(std::string filename);
};

#endif
