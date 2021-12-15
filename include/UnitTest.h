#ifndef UNITTEST
#define UNITTEST

#include <iostream>
#include "cppunit/TestCase.h"

#pragma once

using namespace std;

class UnitTest : public CppUnit::TestCase
{
private:
    /* data */
public:
    UnitTest(string name) : CppUnit::TestCase(name) {}
    ~UnitTest();
};


#endif