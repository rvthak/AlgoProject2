#include <fstream>
#include <iostream>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

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

#define MAX_DISTANCE_OFFSET 0.400

static FILE* test_results = NULL;

int init_suite();
int clean_suite();

Vector* input_vector_tmp;
Vector* query_vector_tmp;

Curve* input_curve_tmp;
Curve* query_curve_tmp;

void test_discrete_distance();

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

	// Create Timers to time the tests
	while(running)
	{
		CU_pSuite pSuite = NULL;

   		if (CU_initialize_registry() != CUE_SUCCESS)
      		return CU_get_error();

		pSuite = CU_add_suite("Suite_Distances", init_suite, clean_suite);
   		
		if (pSuite == NULL) 
		{
      		CU_cleanup_registry();
      		return CU_get_error();
   		}

		if (CU_add_test(pSuite, "Test Of Discrete Distance", test_discrete_distance) == NULL)
		{
			CU_cleanup_registry();
			return CU_get_error();
		}

		CU_basic_set_mode(CU_BRM_VERBOSE);
		CU_basic_run_tests();
		CU_cleanup_registry();
		return CU_get_error();
	}

	print_footer();
	return 0;
}


int init_suite()
{
	if ((test_results = fopen("test_results.txt", "w+")) == NULL)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int clean_suite()
{
	if (fclose(test_results) != 0)
	{
		return -1;
	}
	else
	{
		test_results = NULL;
		return 0;
	}
}

void test_discrete_distance()
{
	input_curve_tmp = input_vector_tmp->create_Curve();
	query_curve_tmp = query_vector_tmp->create_Curve();

	double distance_chris = input_vector_tmp->discrete_frechet_distance(query_vector_tmp);
	double distance_fred = Frechet::Discrete::distance(*input_curve_tmp, *query_curve_tmp).value;

	CU_ASSERT(abs(distance_chris - distance_fred) <= MAX_DISTANCE_OFFSET);
}