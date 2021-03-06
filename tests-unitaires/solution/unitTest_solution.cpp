///
/// @file unitTest_solution.hpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief
///

#include "../unitTest.h"

#ifndef UNITTEST_SOLUTIONARRAY_H
#define UNITTEST_SOLUTIONARRAY_H

#include "solution/solution.h"

using namespace CppUnit;
using namespace stocos;

class UnitTest_solution : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(UnitTest_solution);
    CPPUNIT_TEST(sizeArray);
    CPPUNIT_TEST(operatorEQ);
    CPPUNIT_TEST(asJson);
    CPPUNIT_TEST_SUITE_END();

  public:
    void setUp(void) {
    }
    void tearDown(void) {
    }

    void operatorEQ(void) {
        Solution<double, unsigned int> s1(10);
        Solution<double, unsigned int> s2(10);
        for(unsigned int i = 0; i < 10; i++) {
            s1.setFitness(i);
            s2.setFitness(10 - i);
        }

        s1 = s2;
        for(unsigned int i = 0; i < 10; i++) {
            CPPUNIT_ASSERT(s1.getFitness() == s2.getFitness());
            CPPUNIT_ASSERT(s2.fitnessIsValid() == true);
        }
    }

    void sizeArray(void) {
        Solution<double, unsigned int> s1(100, 2);
        CPPUNIT_ASSERT(s1.sizeArray() == 100);

        Solution<double, unsigned int> s2(s1);
        CPPUNIT_ASSERT(s2.sizeArray() == 100);

        Solution<double, unsigned int> s3(1000, 20);
        s3 = s1;
        CPPUNIT_ASSERT(s3.sizeArray() == 100);
    }

    void asJson(void) {
        Solution<double, unsigned int> s1(10);
        Solution<double, unsigned int> s2(10);
        Solution<double, unsigned int> s3(10);
        s1.setFitness(1);
        s2.setFitness(9);
        for(unsigned int i = 0; i < 10; i++) { s1(i, i); }

        s2.loadJson(s1.asJson());
        for(unsigned int i = 0; i < 10; i++) { CPPUNIT_ASSERT(s1(i) == s2(i)); }
        CPPUNIT_ASSERT(s1.getFitness() == s2.getFitness());

        s3.loadJson(Json::writeString(Json::StreamWriterBuilder(), s1.asJson()));
        for(unsigned int i = 0; i < 10; i++) { CPPUNIT_ASSERT(s1(i) == s3(i)); }
        CPPUNIT_ASSERT(s1.getFitness() == s3.getFitness());

        Json::Value x = s1.asJson();
        Solution<double, unsigned int> s4(x);
        for(unsigned int i = 0; i < 10; i++) { CPPUNIT_ASSERT(s1(i) == s4(i)); }
        CPPUNIT_ASSERT(s1.getFitness() == s4.getFitness());
    }

  private:
};

CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest_solution);

#endif