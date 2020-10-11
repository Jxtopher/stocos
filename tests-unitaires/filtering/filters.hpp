///
/// @file unitTest_criteriaBudget.hpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief
///

#ifndef UNITTEST_FILTERS_H
#define UNITTEST_FILTERS_H

#include <typeinfo>

using namespace CppUnit;

#include "filtering/filters.h"
#include "solution/solution.h"

using namespace stocos;

class UnitTest_filters : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(UnitTest_filters);
	CPPUNIT_TEST(run);
	CPPUNIT_TEST_SUITE_END();

  public:
	static bool checks1_validity_solution(const Solution<double>& s) {
		return true;
	}

	static bool checks2_validity_solution(const Solution<double>& s) {
		return false;
	}

	void run(void) {
		Solution<double> s1(10);
		Filters<Solution<double>> filters;
		filters.insert(checks1_validity_solution);
		CPPUNIT_ASSERT(filters(s1));

		filters.insert(checks2_validity_solution);
		CPPUNIT_ASSERT(filters(s1) == false);
	}
};

#endif