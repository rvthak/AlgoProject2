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

#include "cppunit/TestCaller.h"
#include "cppunit/TestSuite.h"
#include "cppunit/TestResult.h"
#include "cppunit/ui/text/TestRunner.h"

#include "DistanceUnitTest.h"

int main(int argc, char *argv[])
{
	bool running = true;
	print_header();

	// Enter the main program loop
	while (running)
	{
		CppUnit::TextUi::TestRunner runner;

		runner.addTest(DistanceUnitTest::suite());

		runner.run();

		running = !question(" Would you like to exit the program?");
	}

	print_footer();
	return 0;
}