#ifndef ARGS
#define ARGS

#include <string>

// The values of the "Empty" struct data members
#define EMPTY_FILE    ""
#define EMPTY_STRING  ""
#define EMPTY_INT     -1
#define EMPTY_FLOAT   -1

// Argument Storage Struct for LSH
struct ARGS_LSH{
	std::string input_file, query_file, output_file;
	int k;		// Amount of LSH functions (hi) used to create the g functios
	int L;		// Amount of Hash tables used
	int N;		// Amount of Nearest Neighbors
	double R;	// Radius length

	// Initialize all args to "Empty" using an initializer list
	ARGS_LSH():input_file(EMPTY_FILE), query_file(EMPTY_FILE), output_file(EMPTY_FILE), 
	           k(EMPTY_INT), L(EMPTY_INT), N(EMPTY_INT), R(EMPTY_FLOAT){}

	void read_terminal(int argc, char *argv[]);
	void read_args();
	void load_defaults();
	void clear();
	void print();
};

// Argument Storage Struct for Cube
struct ARGS_Cube{
	std::string input_file, query_file, output_file;
	int k;		// The Projection Dimention
	int M;		// Max amount of points checked
	int probes;	// Max amount of cube vertices checked
	int N;		// Amount of Nearest Neighbors
	double R;	// Radius length

	// Initialize all args to "Empty" using an initializer list
	ARGS_Cube():input_file(EMPTY_FILE), query_file(EMPTY_FILE), output_file(EMPTY_FILE), 
	           k(EMPTY_INT), M(EMPTY_INT), probes(EMPTY_INT), N(EMPTY_INT), R(EMPTY_FLOAT){}

	void read_terminal(int argc, char *argv[]);
	void read_args();
	void load_defaults();
	void clear();
	void print();
};

// Argument Storage Struct for Cluster
struct ARGS_Cluster{
	std::string input_file, config_file, output_file;
	std::string method;	// The method that will be used
	bool complete;		// Flag to enable/disable full cluster output

	int k;		// Amount of clusters
	int L;		// Amount of Hash Tables
	int k_lsh;	// Amount of Hash Functions (hi)
	int M;		// Max amount of Vectors checked
	int k_cube;	// Cube Projection Dimention
	int probes;	// Max amount of cube vertices checked

	// Initialize all args to "Empty" using an initializer list
	ARGS_Cluster():input_file(EMPTY_FILE), config_file(EMPTY_FILE), output_file(EMPTY_FILE), 
	               method(EMPTY_STRING), complete(false), k(EMPTY_INT), L(EMPTY_INT),
	               k_lsh(EMPTY_INT), M(EMPTY_INT), k_cube(EMPTY_INT), probes(EMPTY_INT){}

	void read_terminal(int argc, char *argv[]);
	void load_config(std::string filename);
	void load_defaults();
	void print();
};

//------------------------------------------------------------------------------------------------------------------

struct ARGS_Search{
	std::string input_file, query_file, output_file;
	std::string algorithm; // The algorithm that will be used

	int k;		// The Projection Dimention
	int L;		// Amount of Hash tables used
	int M;		// Max amount of points checked
	int probes;	// Max amount of cube vertices checked

	std::string metric;	// The metric that will be used for frechet distance
	double delta;		// Delta value used for grid

	bool notTrue;	// Flag that disables the true nearest neighbour calculation

	// Initialize all args to "Empty" using an initializer list
	ARGS_Search():input_file(EMPTY_FILE), query_file(EMPTY_FILE), output_file(EMPTY_FILE), algorithm(EMPTY_FILE),
	              k(EMPTY_INT), L(EMPTY_INT), M(EMPTY_INT), probes(EMPTY_INT), metric(EMPTY_FILE), delta(EMPTY_FLOAT), notTrue(false){}

	void read_terminal(int argc, char *argv[]);
	void read_args();
	void load_defaults();
	void clear();
	void print();
};

// Argument Storage Struct for Curve Clustering
struct ARGS_Curve_Cluster{
	std::string input_file, config_file, output_file;
	std::string update;		// The method that will be used to update the cluster mean
	std::string assignment;	// The method that will be used to assign Curves to Centroids
	bool complete;			// Flag to enable/disable full cluster output
	bool silhouette;		// Flag to enable/disable silhouette calculation in the output file
	
	int k;		// Amount of clusters
	int L;		// Amount of Hash Tables
	int k_lsh;	// Amount of Hash Functions (hi)
	int M;		// Max amount of Vectors checked
	int k_cube;	// Cube Projection Dimention
	int probes;	// Max amount of cube vertices checked

	// Initialize all args to "Empty" using an initializer list
	ARGS_Curve_Cluster():input_file(EMPTY_FILE), config_file(EMPTY_FILE), output_file(EMPTY_FILE), 
	               update(EMPTY_STRING), assignment(EMPTY_STRING), complete(false), silhouette(false), 
	               k(EMPTY_INT), L(EMPTY_INT), k_lsh(EMPTY_INT), M(EMPTY_INT), k_cube(EMPTY_INT), probes(EMPTY_INT){}

	void read_terminal(int argc, char *argv[]);
	void load_config(std::string filename);
	void load_defaults();
	void ensure_compatability();
	void print();
};

#endif
