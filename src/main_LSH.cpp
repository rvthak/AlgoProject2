#include <fstream>
#include <iostream>
#include "Args.h"
#include "List.h"
#include "utils.h"
#include "timer.h"
#include "Vector.h"
#include "hash_lsh.h"
#include "shortedList.h"

//------------------------------------------------------------------------------------------------------------------

// Variables Used for Performance Statistics
double ratio_sum=0;
unsigned long count=0;

std::vector<double> dist_divergence;

double lsh_time_sum=0;
double range_time_sum=0;
double true_time_sum=0;
unsigned long time_count=0;

void reset_stats();
void report_statistics(double total_time);
void report_results(std::string filename, unsigned id, 
	                ShortedList *lsh,   double lsh_t, 
	                ShortedList *naive, double naive_t, 
	                List *range,        double range_t);

//------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]){
	bool running = true;
	print_header();

	// A struct to store the program parameters
	ARGS_LSH args;

	// Read the given terminal args (if any), and store any args you find
	args.read_terminal(argc, argv);
	args.load_defaults();
	args.print();

	// Create a Timer to time the tests
	Timer t, timer;
	double lsh_time, naive_time, range_time;

	// Pointers to the test results
	ShortedList *lsh_results, *naive_results;
	List *r_results;

	// Enter the main program loop
	while(running){

		// Ask for args (Asks only for "Empty" args)
		args.read_args();

		std::cout << "\033[36;1m (I)\033[33;1m Creating Structs and Loading Data... " << std::endl;
		t.tic();
		// Load both the input and query file data
		VectorArray input_vecs(args.input_file);
		VectorArray query_vecs(args.query_file);

		// Create the LSH Structs
		MultiHash lsh(args.k, args.L, getFileLines(args.input_file)/DIVISION_SIZE, getFileLineLength(args.input_file)-1);

		// Load the input data into the structs
		lsh.loadVectors(&input_vecs);
		// lsh.print();
		//std::cout << " Average Vectors per Bucket: " << lsh.averageBucketSize() << std::endl;
		print_structs_created(t.toc());

		t.tic();
		// For each query Vector:
		for(unsigned i=0; i<(query_vecs.size); i++){

			Vector *q = &((query_vecs.array)[i]);

			// Run and time the tests
			timer.tic();  lsh_results   = lsh.kNN_lsh( q , args.N );                            lsh_time   = timer.toc();
			timer.tic();  naive_results = (ShortedList *)(input_vecs.kNN_naive( q , args.N ));  naive_time = timer.toc();
			timer.tic(); r_results = lsh.range_search( q , args.R );                            range_time = timer.toc();

			// Write a report on the output file
			report_results(args.output_file, q->id, lsh_results, lsh_time, naive_results, naive_time, r_results, range_time);

			// Results written in output file => Free the memory
			delete lsh_results; delete r_results; delete naive_results;
			//std::cout << " > Query " << q->id << std::endl;
		} std::cout << std::endl;

		// Print Out Performance Stats
		report_statistics( t.toc() );

		// Clear the old args and reset the stats
		args.clear();
		reset_stats();

		// Ask user if he wants to stop the program
		running = !question(" Would you like to exit the program?");
	}
	print_footer();
	return 0;
}

//------------------------------------------------------------------------------------------------------------------

// Function used to Report the test results 
void report_results(std::string filename, unsigned id, 
	                ShortedList *lsh,   double lsh_t, 
	                ShortedList *naive, double naive_t, 
	                List *range,        double range_t){

	unsigned i=1;
 	std::ofstream file;

 	// Open the output file in append mode 
	file.open(filename, std::ios_base::app);

	// Write the query id
	file << "Query: " << id << std::endl;

	// Write the results for each "i-th" Neighbor found (Comparing LSH and Naive-True)
	SL_Node *lsh_p = lsh->first, *naive_p = naive->first;
	while( lsh_p != nullptr ){
		file << "Nearest neighbor-" << i++ << ": " << lsh_p->v->id << std::endl;
		file << "distanceLSH:  "  << lsh_p->dist   << std::endl;
		file << "distanceTrue: " << naive_p->dist << std::endl;

		ratio_sum += lsh_p->dist / naive_p->dist;
		count++;
		dist_divergence.push_back( lsh_p->dist / naive_p->dist );

		lsh_p   = lsh_p->next;
		naive_p = naive_p->next;
	}

	// Write the test times
	file << "tLSH:  "  << lsh_t   << std::endl;
	file << "tTrue: " << naive_t << std::endl;

	lsh_time_sum += lsh_t; true_time_sum += naive_t; range_time_sum += range_t;
	time_count++;

	// Write the Range search results 
	file << "R-near neighbors:" << std::endl;
	List_node *cur = range->first;
	while( cur != nullptr ){
		file << cur->data->id << std::endl;
		cur = cur->next;
	}
  	file << std::endl;
}


void report_statistics(double total_time){

	// Calculate the distance Divergence Mean
	double mean_div = ratio_sum/count;

	// Calculate the distance Divergence Variance
	double variance_div=0, tmp;

	for(unsigned i=0; i<count; i++){
		tmp = dist_divergence[i] - mean_div;
		variance_div += tmp*tmp;
	}
	variance_div = variance_div/(count-1);

	// Calculate the mean lsh time
	double mean_lsh = lsh_time_sum/time_count;

	// Calculate the mean true time
	double mean_true = true_time_sum/time_count;

	// Calculate the mean true time
	double range_true = range_time_sum/time_count;

	std::cout << "\033[36;1m (D) \033[33;1mDistance Divergence Mean:     \033[0m" << mean_div << std::endl;
	std::cout << "\033[36;1m (D) \033[33;1mDistance Divergence Variance: \033[0m" << variance_div << std::endl << std::endl;

	std::cout << "\033[36;1m (T) \033[33;1mMean lsh time:   \033[0m" << mean_lsh << " \033[33;1msec\033[0m" << std::endl;
	std::cout << "\033[36;1m (T) \033[33;1mMean true time:  \033[0m" << mean_true << " \033[33;1msec\033[0m" << std::endl;
	std::cout << "\033[36;1m (T) \033[33;1mMean range time: \033[0m" << range_true << " \033[33;1msec\033[0m" << std::endl;
	std::cout << "\033[36;1m (T) \033[33;1mTotal time:      \033[0m" << total_time << " \033[33;1msec\033[0m" << std::endl << std::endl;

	std::cout << "\033[36;1m (R) \033[33;1mlsh to true ratio:   \033[0m" << mean_true/mean_lsh << std::endl;
	std::cout << "\033[36;1m (R) \033[33;1mrange to true ratio: \033[0m" << mean_true/range_true << std::endl << std::endl;
}


void reset_stats(){
	count=0;
	ratio_sum=0;
	time_count=0;
	lsh_time_sum=0;
	true_time_sum=0;
	range_time_sum=0;
	dist_divergence.clear();
}
