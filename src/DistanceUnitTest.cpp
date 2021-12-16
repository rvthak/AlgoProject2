#include <iostream>
#include <vector>
#include "DistanceUnitTest.h"
#include "cppunit/TestCase.h"

#include "Vector.h"
#include "VCurve.h"
#include "curve.hpp"
#include "frechet.hpp"

using namespace std;

void DistanceUnitTest::setUp()
{
    // VectorArray input_vecs("./datasets/test_input.csv");
	// VectorArray query_vecs("/datasets/test_query.csv");

    // VectorArray input_vecs("./datasets/nasd_input.csv");
	// VectorArray query_vecs("./datasets/nasd_query.csv");

    std::cout << "Started setup" << std::endl;

    vector<double> input_v_1_1 {34.5};
    vector<double> query_v_1_1 {83.4};



    Vector input_vector_1_1(input_v_1_1);
        std::cout << "Initialized vector" << std::endl;

	this->input_vector_1_1 = &input_vector_1_1; 
    std::cout << "Initialized vector" << std::endl;
	
    
    // this->query_vector_1_1->vec(query_v_1_1);

    // this->input_vector_1_2 = &input_vecs.array[1];
	// this->query_vector_1_2 = &query_vecs.array[1];

    // this->input_vector_2_1 = &input_vecs.array[2];
	// this->query_vector_2_1 = &query_vecs.array[2];

    // this->input_vector_m_n = &input_vecs.array[3];
	// this->query_vector_m_n = &query_vecs.array[3];

    this->input_curve_1_1 = this->input_vector_1_1->create_Curve();
    this->query_curve_1_1 = this->query_vector_1_1->create_Curve();

    this->input_curve_1_2 = this->input_vector_1_2->create_Curve();
    this->query_curve_1_2 = this->query_vector_1_2->create_Curve();

    this->input_curve_2_1 = this->input_vector_2_1->create_Curve();
    this->query_curve_2_1 = this->query_vector_2_1->create_Curve();

    this->input_curve_m_n = this->input_vector_m_n->create_Curve();
    this->query_curve_m_n = this->query_vector_m_n->create_Curve();

    std::cout << "Finished setup" << std::endl;
}


void DistanceUnitTest::test_discrete_distance()
{
    std::cout << "Started test" << std::endl;

    double distance_chris_1_1 = this->input_vector_1_1->discrete_frechet_distance(this->query_vector_1_1);
    double distance_fred_1_1 = Frechet::Discrete::distance(*this->input_curve_1_1, *this->query_curve_1_1).value;

    double distance_chris_1_2 = this->input_vector_1_2->discrete_frechet_distance(this->query_vector_1_2);
    double distance_fred_1_2 = Frechet::Discrete::distance(*this->input_curve_1_2, *this->query_curve_1_2).value;
    
    double distance_chris_2_1 = this->input_vector_2_1->discrete_frechet_distance(this->query_vector_2_1);
    double distance_fred_2_1 = Frechet::Discrete::distance(*this->input_curve_2_1, *this->query_curve_2_1).value;

    double distance_chris_m_n = this->input_vector_m_n->discrete_frechet_distance(this->query_vector_m_n);
    double distance_fred_m_n = Frechet::Discrete::distance(*this->input_curve_m_n, *this->query_curve_m_n).value;

    std::cout << "Going to asserts" << std::endl;

    CPPUNIT_ASSERT(distance_chris_1_1 == distance_fred_1_1);
    CPPUNIT_ASSERT(distance_chris_1_2 == distance_fred_1_2);
    CPPUNIT_ASSERT(distance_chris_2_1 == distance_fred_2_1);
    CPPUNIT_ASSERT(distance_chris_m_n == distance_fred_m_n);
}