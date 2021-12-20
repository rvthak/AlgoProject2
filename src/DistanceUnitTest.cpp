#include <iostream>
#include <vector>
#include "DistanceUnitTest.h"

#include "cppunit/TestCase.h"
#include "cppunit/TestSuite.h"
#include "cppunit/TestCaller.h"

#include "Vector.h"
#include "VCurve.h"
#include "curve.hpp"
#include "utils.h"
#include "frechet.hpp"

using namespace std;

void DistanceUnitTest::test_discrete_distance_1_1()
{
    cout << "Test I : 1 point Q : 1 point" << endl;

    vector<double> input_v_1_1 { 34.5 };
    vector<double> query_v_1_1 { 83.4 };

    Vector input_vector_1_1(input_v_1_1);
    this->input_vector_1_1 = &input_vector_1_1;

    Vector query_vector_1_1(query_v_1_1);
    this->query_vector_1_1 = &query_vector_1_1;

    this->input_curve_1_1 = this->input_vector_1_1->create_Curve();
    this->query_curve_1_1 = this->query_vector_1_1->create_Curve();

    double distance_chris_1_1 = this->input_vector_1_1->discrete_frechet_distance(this->query_vector_1_1);
    double distance_fred_1_1 = Frechet::Discrete::distance(*this->input_curve_1_1, *this->query_curve_1_1).value;

    CPPUNIT_ASSERT(distance_chris_1_1 == distance_fred_1_1);

    cout << "Test Passed!" << endl;
}

void DistanceUnitTest::test_discrete_distance_1_2()
{
    cout << "Test I : 1 point Q : 2 points" << endl;

    vector<double> input_v_1_2 { 78.9 };
    vector<double> query_v_1_2 { 11.6, 32.1 };

    Vector input_vector_1_2(input_v_1_2);
    this->input_vector_1_2 = &input_vector_1_2;

    Vector query_vector_1_2(query_v_1_2);
    this->query_vector_1_2 = &query_vector_1_2;

    this->input_curve_1_2 = this->input_vector_1_2->create_Curve();
    this->query_curve_1_2 = this->query_vector_1_2->create_Curve();

    double distance_chris_1_2 = this->input_vector_1_2->discrete_frechet_distance(this->query_vector_1_2);
    double distance_fred_1_2 = Frechet::Discrete::distance(*this->input_curve_1_2, *this->query_curve_1_2).value;

    CPPUNIT_ASSERT(distance_chris_1_2 == distance_fred_1_2);

    cout << "Test Passed!" << endl;
}  

void DistanceUnitTest::test_discrete_distance_2_1()
{
    cout << "Test I : 2 points Q : 1 point" << endl;

    vector<double> input_v_2_1 { 58.3, 49.1 };
    vector<double> query_v_2_1 { 90.4 };

    Vector input_vector_2_1(input_v_2_1);
    this->input_vector_2_1 = &input_vector_2_1;

    Vector query_vector_2_1(query_v_2_1);
    this->query_vector_2_1 = &query_vector_2_1;

    this->input_curve_2_1 = this->input_vector_2_1->create_Curve();
    this->query_curve_2_1 = this->query_vector_2_1->create_Curve();

    double distance_chris_2_1 = this->input_vector_2_1->discrete_frechet_distance(this->query_vector_2_1);
    double distance_fred_2_1 = Frechet::Discrete::distance(*this->input_curve_2_1, *this->query_curve_2_1).value;
    
    CPPUNIT_ASSERT(distance_chris_2_1 == distance_fred_2_1);

    cout << "Test Passed!" << endl;
}

void DistanceUnitTest::test_discrete_distance_m_n()
{
    cout << "Test I : m points Q : n points" << endl;

    vector<double> input_v_m_n { 56.3, 03.2, 78.2, 67.1, 31,4 };
    vector<double> query_v_m_n { 67.2, 18.4, 32.8, 59.2, 67.3, 90.2, 41.5 };

    Vector input_vector_m_n(input_v_m_n);
    this->input_vector_m_n = &input_vector_m_n;

    Vector query_vector_m_n(query_v_m_n);
    this->query_vector_m_n = &query_vector_m_n;

    this->input_curve_m_n = this->input_vector_m_n->create_Curve();
    this->query_curve_m_n = this->query_vector_m_n->create_Curve();

    double distance_chris_m_n = this->input_vector_m_n->discrete_frechet_distance(this->query_vector_m_n);
    double distance_fred_m_n = Frechet::Discrete::distance(*this->input_curve_m_n, *this->query_curve_m_n).value;

    CPPUNIT_ASSERT(distance_chris_m_n == distance_fred_m_n);

    cout << "Test Passed!" << endl;
}

CppUnit::Test* DistanceUnitTest::suite()
{
    CppUnit::TestSuite *test_suite = new CppUnit::TestSuite("DistanceUnitTest");

    test_suite->addTest(new CppUnit::TestCaller<DistanceUnitTest>("discrete_distance_test_1_1", &DistanceUnitTest::test_discrete_distance_1_1));
    test_suite->addTest(new CppUnit::TestCaller<DistanceUnitTest>("discrete_distance_test_1_2", &DistanceUnitTest::test_discrete_distance_1_2));
    test_suite->addTest(new CppUnit::TestCaller<DistanceUnitTest>("discrete_distance_test_2_1", &DistanceUnitTest::test_discrete_distance_2_1));
    test_suite->addTest(new CppUnit::TestCaller<DistanceUnitTest>("discrete_distance_test_m_n", &DistanceUnitTest::test_discrete_distance_m_n));

    return test_suite;
}
