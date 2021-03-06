///
/// @file unitTest_filtering.cpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief g++ tests-unitaires/filtering/test.cpp -I sources/ -Iinclude -lcppunit -lpthread
/// -lboost_log -lboost_system -ljsoncpp
///

#include "../unitTest.h"

#ifndef UNITTEST_FILTERS_H
#define UNITTEST_FILTERS_H

#include "filtering/filters.h"
#include "solution/fitness.h"

using namespace stocos;

class UnitTest_filters : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(UnitTest_filters);
    CPPUNIT_TEST(run);
    CPPUNIT_TEST_SUITE_END();

  public:
    static bool checks1_validity_solution(const Fitness<double>& s) {
        return true;
    }

    static bool checks2_validity_solution(const Fitness<double>& s) {
        return false;
    }

    void run(void) {
        Fitness<double> s1(10);
        Filters<Fitness<double>> filters;
        filters.insert(checks1_validity_solution);
        CPPUNIT_ASSERT(filters(s1));

        filters.insert(checks2_validity_solution);
        CPPUNIT_ASSERT(filters(s1) == false);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest_filters);

#endif