#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "Args.h"
#include "timer.h"
#include "utils.h"
#include "Vector.h"
#include "shortedList.h"
#include "hash_lsh.h"
#include "hash_cube.h"
#include "GridHash.h"

#include "point.hpp"
#include "curve.hpp"
#include "frechet.hpp"

int main(int argc, char *argv[])
{
	bool running = true;
	print_header();

	// A struct to store the program parameters
	ARGS_Search args;

	// Read the given terminal args (if any), and store any args you find
	args.read_terminal(argc, argv);
	args.load_defaults();
	args.print();

	// Enter the main program loop
	while (running)
	{
		// Ask for args (Asks only for "Empty" args)
		args.read_args();
		std::cout << "\033[36;1m (I)\033[33;1m Creating Structs and Loading Data... " << std::endl;

		// Load both the input and query file data
		VectorArray input_vecs(args.input_file);
		VectorArray query_vecs(args.query_file);
		//query_vecs.print();

		Vector* input_vector_1 = &input_vecs.array[0];
		Vector* query_vector_1 = &query_vecs.array[0];

		Curve* input_curve_1 = input_vector_1->create_Curve();
		Curve* query_curve_1 = query_vector_1->create_Curve();

		// Initialize Unit Tests

		// Clear the old args and reset the stats
		args.clear();

		// Ask user if he wants to stop the program
		running = !question(" Would you like to exit the program?");
	}

	print_footer();
	return 0;
}