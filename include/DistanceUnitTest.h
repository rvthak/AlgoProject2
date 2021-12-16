#ifndef DISTANCEUNITTEST
#define DISTANCEUNITTEST

#include <iostream>
#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"

#include "Vector.h"
#include "VCurve.h"
#include "curve.hpp"

#define MAX_DISTANCE_OFFSET 200.0  // CHRIS 13.12.2021 START

#pragma once

using namespace std;

class DistanceUnitTest : public CppUnit::TestFixture
{
private:

    double max_distance_offset = MAX_DISTANCE_OFFSET;
    
    Vector* input_vector_1_1;
    Vector* query_vector_1_1;

    Vector* input_vector_1_2;
    Vector* query_vector_1_2;

    Vector* input_vector_2_1;
    Vector* query_vector_2_1;

    Vector* input_vector_m_n;
    Vector* query_vector_m_n;

    Curve* input_curve_1_1;
    Curve* query_curve_1_1;

    Curve* input_curve_1_2;
    Curve* query_curve_1_2;

    Curve* input_curve_2_1;
    Curve* query_curve_2_1;

    Curve* input_curve_m_n;
    Curve* query_curve_m_n;

public:

    void setUp();
    void test_discrete_distance();
};


#endif