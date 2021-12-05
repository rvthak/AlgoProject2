#include <fstream>
#include <iostream>
#include "Args.h"
#include "timer.h"
#include "utils.h"
#include "Vector.h"
#include "shortedList.h"

//------------------------------------------------------------------------------------------------------------------

double approx_time_sum=0, true_time_sum=0;
double MAF = 0; // Max Approximation Factor
unsigned count=0;

void reset_stats();
void report_statistics(std::string filename);
void report_results(std::string filename, std::string id, std::string algo, 
                    ShortedList *approx_r, double approx_time, 
                    ShortedList *true_r, double true_time);

//------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]){
	bool running = true;
	print_header();

	// A struct to store the program parameters
	ARGS_Search args;

	// Read the given terminal args (if any), and store any args you find
	args.read_terminal(argc, argv);
	args.load_defaults();
	args.print();

	// Enter the main program loop
	while(running){

		// Ask for args (Asks only for "Empty" args)
		args.read_args();
		std::cout << "\033[36;1m (I)\033[33;1m Creating Structs and Loading Data... " << std::endl;

		// Load both the input and query file data
		VectorArray input_vecs(args.input_file);
		VectorArray query_vecs(args.query_file);
		input_vecs.print();

		// Clear the old args and reset the stats
		args.clear();

		// Ask user if he wants to stop the program
		running = !question(" Would you like to exit the program?");
	}
	print_footer();
	return 0;
}

//------------------------------------------------------------------------------------------------------------------

// Write a report of the results for the given id on the output file
void report_results(std::string filename, std::string id, std::string algo, ShortedList *approx_r, double approx_time, ShortedList *true_r, double true_time){
	double cur_maf;

	// Open the output file in append mode 
 	std::ofstream file(filename, std::ios_base::app);

	// Write the query results
	file << "Query: " << id << std::endl;
	file << "Algorithm: " << algo << std::endl;

	file << "Approximate Nearest neighbor: " << approx_r->first->v->name << std::endl;
	file << "True Nearest neighbor: " << true_r->first->v->name << std::endl;

	file << "distanceApproximate: " << approx_r->first->dist << std::endl;
	file << "distanceTrue: " << true_r->first->dist << std::endl;

	// Update any needed stat variables
	approx_time_sum+=approx_time;
	true_time_sum+=true_time;
	
	cur_maf = true_time/approx_time;
	if( cur_maf > MAF ){ MAF = cur_maf; }
	count++;

  	file << std::endl;
}

// Write the final statistics report on the output file
void report_statistics(std::string filename){
	// Open the output file in append mode 
	std::ofstream file(filename, std::ios_base::app);

 	file << "tApproximateAverage: " << approx_time_sum/count << std::endl;
 	file << "tTrueAverage: " << true_time_sum/count << std::endl;
 	file << "MAF: " << MAF << std::endl;
}

// Reset the values of the stat counters before the next run
void reset_stats(){
	approx_time_sum=0;
	true_time_sum=0;
	MAF = 0;
	count=0;
}
