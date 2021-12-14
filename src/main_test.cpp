#include <fstream>
#include <iostream>
#include "Args.h"
#include "timer.h"
#include "utils.h"
#include "Vector.h"
#include "shortedList.h"
#include "hash_lsh.h"
#include "hash_cube.h"
#include "GridHash.h"

//------------------------------------------------------------------------------------------------------------------

double approx_time_sum=0, true_time_sum=0;
double MAF = 0; // Max Approximation Factor
unsigned count=0;

void reset_stats();
void report_statistics(std::string filename, bool ignoreTrue, double total_time);
void report_results(std::string filename, std::string id, std::string algo, bool ignoreTrue, 
                    ShortedList *approx_r, double approx_time, 
                    ShortedList *true_r, double true_time);

void clear_results(ShortedList **approx_res, ShortedList **true_res);
void garbage_collector(MultiHash *lsh, Hypercube *cube, GridHash  *grid);

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

	// Create Timers to time the tests
	Timer t, timer;
	double approx_time, true_time;

	// Pointers to the query results
	ShortedList *approx_results=nullptr, *true_results=nullptr;

	// Enter the main program loop
	while(running){

		// Ask for args (Asks only for "Empty" args)
		args.read_args();
		std::cout << "\033[36;1m (I)\033[33;1m Creating Structs and Loading Data... " << std::endl;
		t.tic();

		// Load both the input and query file data
		VectorArray input_vecs(args.input_file);
		VectorArray query_vecs(args.query_file);
		//query_vecs.print();

		// Build the user-selected Data structures
		MultiHash *lsh  = nullptr;
		Hypercube *cube = nullptr;
		GridHash  *grid = nullptr;

		if( args.algorithm == "LSH" ){
			lsh = new MultiHash(args.k, args.L, getFileLines(args.input_file)/DIVISION_SIZE, getFileLineLength(args.input_file)-1);			
			lsh->loadVectors(&input_vecs);
		}
		else if( args.algorithm == "Hypercube" ){
			cube = new Hypercube(args.k, getFileLines(args.input_file)/DIVISION_SIZE_CUBE, getFileLineLength(args.input_file)-1);
			cube->set_search_limits(args.probes, args.M, args.k);
			cube->loadVectors(&input_vecs);
		}
		else{ // Frechet
			unsigned dim; // The projection dimention
			if( args.metric == "discrete"){ dim=2; }
			else { dim=1; }

			grid = new GridHash(args.delta, args.L, dim, args.k, getFileLines(args.input_file)/DIVISION_SIZE, getFileLineLength(args.input_file)-1);
			grid->loadVectors(&input_vecs);
		}
		print_structs_created(t.toc());

		//------------------------------------------------------------------------------------------------------------------
		
		t.tic();
		// Run tests for each query Vector:
		for(unsigned i=0; i<(query_vecs.size); i++){

			Vector *q = &((query_vecs.array)[i]);

			// Run and time the tests
			if( args.algorithm == "LSH" ){
				timer.tic();  approx_results = lsh->kNN_lsh(q , 1); approx_time = timer.toc();
			} 
			else if( args.algorithm == "Hypercube" ){
				timer.tic(); cube->search_hypercube(q);
				approx_results = cube->k_nearest_neighbors_search(1); approx_time = timer.toc();
			} 
			else if( (args.algorithm == "Frechet") && (args.metric == "discrete") ){
				timer.tic();  approx_results = grid->disc_NN_lsh(q); approx_time = timer.toc();
			}
			else{ // Continuous Frechet
				timer.tic();  approx_results = grid->cont_NN_lsh(q); approx_time = timer.toc();
			}

			if( args.notTrue == false ){
				timer.tic(); true_results = (ShortedList *)(input_vecs.kNN_naive(q , 1)); true_time = timer.toc();
			}

			// Write a report on the output file
			report_results(args.output_file, q->name, args.algorithm, args.notTrue, approx_results, approx_time, true_results, true_time);

			// Results written in output file => Free the memory
			clear_results(&approx_results, &true_results);
			std::cout << " > Query " << q->id << std::endl;

		} std::cout << std::endl;

		// Print Out Performance Stats
		report_statistics( args.output_file, args.notTrue, t.toc() );

		//------------------------------------------------------------------------------------------------------------------

		// Garbage Collection
		garbage_collector(lsh, cube, grid);

		// Clear the old args and reset the stats
		args.clear(); reset_stats();

		// Ask user if he wants to stop the program
		running = !question(" Would you like to exit the program?");
	}
	print_footer();
	return 0;
}

//------------------------------------------------------------------------------------------------------------------

void garbage_collector(MultiHash *lsh, Hypercube *cube, GridHash  *grid){
	if( lsh  != nullptr ){ delete lsh;  }
	if( cube != nullptr ){ delete cube; }
	if( grid != nullptr ){ delete grid; }
}

void clear_results(ShortedList **approx_res, ShortedList **true_res){
	if( (*approx_res) != nullptr ){ delete *approx_res; (*approx_res) = nullptr; }
	if( (*true_res)   != nullptr ){ delete *true_res;   (*true_res  ) = nullptr; }
}

// Write a report of the results for the given id on the output file
void report_results(std::string filename, std::string id, std::string algo, bool ignoreTrue, ShortedList *approx_r, double approx_time, ShortedList *true_r, double true_time){
	double cur_maf;

	// Open the output file in append mode 
 	std::ofstream file(filename, std::ios_base::app);

	// Write the query results
	file << "Query: " << id << std::endl;
	file << "Algorithm: " << algo << std::endl;

	file << "Approximate Nearest neighbor: " << approx_r->first->v->name << std::endl;
	if( !ignoreTrue ){
		file << "True Nearest neighbor: " << true_r->first->v->name << std::endl;
	}

	file << "distanceApproximate: " << approx_r->first->dist << std::endl;
	if( !ignoreTrue ){
		file << "distanceTrue: " << true_r->first->dist << std::endl;
	}

	// Update any needed stat variables
	approx_time_sum+=approx_time;
	true_time_sum+=true_time;
	count++;

	if( !ignoreTrue ){
		cur_maf = true_time/approx_time;
		if( cur_maf > MAF ){ MAF = cur_maf; }
	}	

  	file << std::endl;
}

// Write the final statistics report on the output file
void report_statistics(std::string filename, bool ignoreTrue, double total_time){
	// Open the output file in append mode 
	std::ofstream file(filename, std::ios_base::app);

 	file << "tApproximateAverage: " << approx_time_sum/count << std::endl;

 	if( !ignoreTrue ){
 		file << "tTrueAverage: " << true_time_sum/count << std::endl;
 		file << "MAF: " << MAF << std::endl;
 	}

 	print_total_time(total_time);
}

// Reset the values of the stat counters before the next run
void reset_stats(){
	approx_time_sum=0;
	true_time_sum=0;
	MAF = 0;
	count=0;
}