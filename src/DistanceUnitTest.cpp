#include <iostream>
#include "DistanceUnitTest.h"
#include "cppunit/TestCase.h"

#include "Vector.h"
#include "VCurve.h"
#include "curve.hpp"
#include "frechet.hpp"

using namespace std;

void DistanceUnitTest::setup_test(Vector* input_vector, Vector* query_vector)
{
    this->input_vector = input_vector;
    this->query_vector = query_vector;

    this->input_curve = input_vector->create_Curve();
    this->query_curve = query_vector->create_Curve();
}

void DistanceUnitTest::setup_test(Curve* input_curve, Curve* query_curve)
{
    this->input_curve = input_curve;
    this->query_curve = query_curve;
}

void DistanceUnitTest::run_distance_test()
{
    double distance_chris = this->input_vector->discrete_frechet_distance(this->query_vector);
    double distance_fred = Frechet::Discrete::distance(*this->input_curve, *this->query_curve).value;

    double distance_offset = abs(distance_chris - distance_fred);

    CPPUNIT_ASSERT(distance_offset <= this->max_distance_offset);
}