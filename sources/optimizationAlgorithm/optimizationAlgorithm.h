///
/// \file optimizationAlgorithm.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2018-10
/// \brief 
///

#ifndef OPTIMIZATIONALGORITHM_H
#define OPTIMIZATIONALGORITHM_H

#include <random>

#include "../stoppingCriteria/stoppingCriteria.h"
#include "../problem/problem.h"
#include "../statistic/statistic.h"

template<class SOL, typename TYPE_CELL>
class OptimizationAlgorithm {
    public:
        OptimizationAlgorithm(std::mt19937 &mt_rand,
        Statistic<SOL> &statistic,
        StoppingCriteria<SOL> &stoppingCriteria,
        Problem<SOL, TYPE_CELL> &problem) : 
            _mt_rand(mt_rand),
            _statistic(statistic),
            _stoppingCriteria(stoppingCriteria),
            _problem(problem) {

        }
        virtual ~OptimizationAlgorithm() {

        }
    
        virtual void operator()(SOL &s) = 0;


    protected:
        std::mt19937 &_mt_rand;
        Statistic<SOL> &_statistic;
        StoppingCriteria<SOL> &_stoppingCriteria;
        Problem<SOL, TYPE_CELL> &_problem;

};

#endif