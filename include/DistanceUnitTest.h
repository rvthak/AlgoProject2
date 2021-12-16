#ifndef DISTANCEUNITTEST
#define DISTANCEUNITTEST

#include <iostream>
#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"

#include "Vector.h"
#include "VCurve.h"
#include "curve.hpp"

#pragma once

using namespace std;

class DistanceUnitTest : public CppUnit::TestFixture
{
private:
    
    vector<double> input_v_1_1 {34.5};
    vector<double> query_v_1_1 {83.4};
    
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