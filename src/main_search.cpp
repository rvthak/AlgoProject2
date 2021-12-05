#include <fstream>
#include <iostream>
#include "Args.h"
#include "timer.h"
#include "utils.h"
#include "Vector.h"

//------------------------------------------------------------------------------------------------------------------

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
		input_vecs.print();
		//VectorArray query_vecs(args.query_file);


		// Clear the old args and reset the stats
		args.clear();

		// Ask user if he wants to stop the program
		running = !question(" Would you like to exit the program?");
	}
	print_footer();
	return 0;
}

//------------------------------------------------------------------------------------------------------------------

