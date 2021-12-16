#include <iostream>
#include "DistanceUnitTest.h"
#include "cppunit/TestCase.h"

#include "Vector.h"
#include "VCurve.h"
#include "curve.hpp"
#include "frechet.hpp"

using namespace std;

void DistanceUnitTest::setUp()
{
    VectorArray input_vecs("./datasets/nasd_input.csv");
	VectorArray query_vecs("./datasets/nasd_query.csv");

	this->input_vector = &input_vecs.array[0];
	this->query_vector = &query_vecs.array[0];

    this->input_curve = input_vector->create_Curve();
    this->query_curve = query_vector->create_Curve();
}

void DistanceUnitTest::setup_test(Curve* input_curve, Curve* query_curve)
{
    this->input_curve = input_curve;
    this->query_curve = query_curve;
}

void DistanceUnitTest::discrete_distance()
{
    double distance_chris = this->input_vector->discrete_frechet_distance(this->query_vector);
    double distance_fred = Frechet::Discrete::distance(*this->input_curve, *this->query_curve).value;

    double distance_offset = abs(distance_chris - distance_fred);

    CPPUNIT_ASSERT(distance_offset <= this->max_distance_offset);
}