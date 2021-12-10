#include <fstream>
#include <iostream>

#include "Args.h"
#include "timer.h"
#include "utils.h"
#include "Vector.h"
#include "shortedList.h"
#include "hash_lsh.h"
#include "hash_cube.h"

#include "point.hpp"
#include "curve.hpp"
#include "frechet.hpp"

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
void garbage_collector(MultiHash *lsh, Hypercube *cube);

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
		//input_vecs.print();

		// Build the user-selected Data structures
		MultiHash *lsh = nullptr;
		Hypercube *cube = nullptr;

		if( args.algorithm == "LSH" ){
			lsh = new MultiHash(args.k, args.L, getFileLines(args.input_file)/DIVISION_SIZE, getFileLineLength(args.input_file)-1);			
			lsh->loadVectors(&input_vecs);
		}
		else if( args.algorithm == "Hypercube" ){
			cube = new Hypercube(args.k, getFileLines(args.input_file)/DIVISION_SIZE_CUBE, getFileLineLength(args.input_file)-1);
			cube->set_search_limits(args.probes, args.M, args.k);
			cube->loadVectors(&input_vecs);
		}
		else if (args.algorithm == "Frechet")
		{ 
			// CHRIS 09.12.2021 FRED INTEGRATION START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //

			cout << "Starting to integrate Fred" << endl;
			cout << "Header files included!" << endl;

			// CHRIS 07.12.2021 START

			if (args.metric == "discrete")
			{
				// int dimensions = 2;

				// Curve* test_curve1 = new Curve(dimensions, "test_curve1");
				// Curve* test_curve2 = new Curve(dimensions, "test_curve2");

				// for (unsigned i = 0; i < input_vecs.array[0].vec.size(); i ++)
				// {
				// 	Point* test_point = new Point(dimensions);

				// 	test_point->set(0 , i);
				// 	test_point->set(1, input_vecs.array[0].vec[i]);

				// 	test_curve1->push_back(*test_point);
					
				// 	// cout << "X : " << test_point->get(0) << " Y : " << test_point->get(1) << endl;
				// }

				// for (unsigned i = 0; i < input_vecs.array[1].vec.size(); i ++)
				// {
				// 	Point* test_point = new Point(dimensions);

				// 	test_point->set(0 , i);
				// 	test_point->set(1, input_vecs.array[1].vec[i]);

				// 	test_curve2->push_back(*test_point);
					
				// 	// cout << "X : " << test_point->get(0) << " Y : " << test_point->get(1) << endl;
				// }

				// Frechet::Discrete::Distance discrete_distance =  Frechet::Discrete::distance(*test_curve1, *test_curve2);

				// cout << "Discrete Frechet Distance between 2 test curves : " << discrete_distance.value << endl;

				// Point* test_point1 = new Point(dimensions);

				// test_point1->set(0 , 0);
				// test_point1->set(1, input_vecs.array[0].vec[0]);

				// Point* test_point2 = new Point(dimensions);

				// test_point2->set(0 , 0);
				// test_point2->set(1, input_vecs.array[0].vec[1]);

				// cout << "Test points created with dimensions : " << test_point1->dimensions() << endl;
				// cout << "And coordinates :" << endl;
				// cout << "X1 : " << test_point1->get(0) << " Y1 : " << test_point1->get(1) << endl;
				// cout << "X2 : " << test_point2->get(0) << " Y2 : " << test_point2->get(1) << endl;
			}
			else if (args.metric == "continuous")
			{
				
			}


			// return 0;
			
			// CHRIS 07.12.2021 END
			// std::cout << " Frechet still under construction " << std::endl;

			// CHRIS 09.12.2021 FRED INTEGRATION END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //
		}
		print_structs_created(t.toc());

		//------------------------------------------------------------------------------------------------------------------
		
		t.tic();
		// Run tests for each query Vector:
		for(unsigned i=0; i<(query_vecs.size); i++){

			Vector *q = &((query_vecs.array)[i]);

			// CHRIS 07.12.2021 START

			// Run and time the tests
			if( args.algorithm == "LSH" )
			{
				timer.tic();  approx_results = lsh->kNN_lsh_discrete_frechet(q , 1); approx_time = timer.toc();
			} 
			else if( args.algorithm == "Hypercube" )
			{
				timer.tic(); cube->search_hypercube(q);
				approx_results = cube->k_nearest_neighbors_search(1); approx_time = timer.toc();
			}
			else
			{
				if (args.metric == "discrete")
				{
					// CHRIS 09.12.2021 FRED INTEGRATION START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //

					timer.tic();

					cout << "Going to initialize curves" << endl;

					Curve* input_curve = new Curve(2, "input_curve");
					Curve* query_curve = new Curve(2, "query_curve");

					cout << "Initialized curves" << endl;

					for (unsigned j = 0; j < input_vecs.array[0].vec.size(); j++)
					{
						Point* input_point = new Point(2);

						input_point->set(0 , i);
						input_point->set(1, input_vecs.array[0].vec[i]);

						input_curve->push_back(*input_point);
					}

					for (unsigned i = 0; i < q->vec.size(); i++)
					{
						Point* query_point = new Point(2);

						query_point->set(0 , i);
						query_point->set(1, q->vec[i]);

						query_curve->push_back(*query_point);
					}

					cout << "Loaded points in curves" << endl;

					Frechet::Discrete::Distance discrete_distance_fred =  Frechet::Discrete::distance(*input_curve, *query_curve);
					double discrete_distance_chris =  q->discrete_frechet_distance(&input_vecs.array[0]);

					cout << "Distance Fred : " << discrete_distance_fred.value << endl;
					cout << "Distance Chris : " << discrete_distance_chris << endl;


					// approx_results = lsh->kNN_lsh_discrete_frechet(q , 1);

					approx_time = timer.toc();

					// CHRIS 09.12.2021 FRED INTEGRATION END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //

				}
				else if (args.metric == "continuous")
				{
					timer.tic();			

					// Use the frechet distance library
					
					approx_time = timer.toc();
				}
			}

			if ( args.notTrue == false )
			{
				timer.tic(); true_results = (ShortedList *)(input_vecs.kNN_naive(q , 1)); true_time = timer.toc();
			}

			// CHRIS 07.12.2021 END

			// Write a report on the output file
			cout << "Going to write results" << endl;
			report_results(args.output_file, q->name, args.algorithm, args.notTrue, approx_results, approx_time, true_results, true_time);

			// Results written in output file => Free the memory
			clear_results(&approx_results, &true_results);
			std::cout << " > Query " << q->id << std::endl;

		} std::cout << std::endl;

		// Print Out Performance Stats
		report_statistics( args.output_file, args.notTrue, t.toc() );

		//------------------------------------------------------------------------------------------------------------------

		// Garbage Collection
		garbage_collector(lsh, cube);

		// Clear the old args and reset the stats
		args.clear(); reset_stats();

		// Ask user if he wants to stop the program
		running = !question(" Would you like to exit the program?");
	}
	print_footer();
	return 0;
}

//------------------------------------------------------------------------------------------------------------------

void garbage_collector(MultiHash *lsh, Hypercube *cube){
	if( lsh  != nullptr ){ delete lsh;  }
	if( cube != nullptr ){ delete cube; }
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
