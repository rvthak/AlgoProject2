#ifndef DISTANCEUNITTEST
#define DISTANCEUNITTEST

#include <iostream>
#include "cppunit/TestCase.h"

#include "Vector.h"
#include "VCurve.h"
#include "curve.hpp"

#define MAX_DISTANCE_OFFSET 200.0  // CHRIS 13.12.2021 START

#pragma once

using namespace std;

class DistanceUnitTest : public CppUnit::TestCase
{
private:
    double max_distance_offset = MAX_DISTANCE_OFFSET;
    
    Vector* input_vector;
    Vector* query_vector;
    Curve* input_curve;
    Curve* query_curve;

public:
    DistanceUnitTest(string name) : CppUnit::TestCase(name) {}
    ~DistanceUnitTest();

    void setUp();
    void setup_test(Curve* input_curve, Curve* query_curve);
    void discrete_distance();
};


#endif