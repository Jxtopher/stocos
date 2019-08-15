///
/// @file firstImprovement.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief 
///
 

#ifndef FIRSTIMPROVEMENT_H
#define FIRSTIMPROVEMENT_H

#include <random>

#include "operator/atomicOperation.h"
#include "../optimizationAlgorithm.h"
#include "../../problem/problem.h"
#include "selection/selection.h"

using namespace std;

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
class FirstImprovement : public OptimizationAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL> {
    public:
    FirstImprovement(std::mt19937 &mt_rand, 
    std::unique_ptr<Statistic<SOL>> statistic,
    std::unique_ptr<StoppingCriteria<SOL, TYPE_FITNESS>> stoppingCriteria,
    shared_ptr<Problem<SOL, TYPE_FITNESS, TYPE_CELL>> problem,
    std::unique_ptr<AtomicOperation<SOL, TYPE_FITNESS, TYPE_CELL>> atomicOperations,
    std::unique_ptr<Selection<SOL>> selection) : 
    OptimizationAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>(mt_rand, move(statistic), move(stoppingCriteria), problem),
    _atomicOperations(move(atomicOperations)),
    _selection(move(selection)) {
        DEBUG_TRACE("Creation FirstImprovement");
    }

    ~FirstImprovement() {
    }
    
    std::unique_ptr<SOL> operator()(const SOL &s) {
        solution_star = s;
        if (!solution_star.fitnessIsValid()) {
            this->_problem->full_eval(solution_star);
        }
        
        while (this->_stoppingCriteria->operator()(solution_star)) {
            this->_statistic->operator()(solution_star);

            solution_beta = solution_star;
            
            _atomicOperations->operator()(solution_beta);
            this->_problem->full_eval(solution_beta);
            if (_selection->operator()(solution_beta, solution_star)) {
                solution_star = solution_beta;
            }
        }

        this->_statistic->operator()(solution_star);
        return move(make_unique<SOL>(solution_star));
    }

    string className() const {
        return "FirstImprovement";
    }
    protected:
        std::unique_ptr<AtomicOperation<SOL, TYPE_FITNESS, TYPE_CELL>> _atomicOperations;
        std::unique_ptr<Selection<SOL>> _selection;
        SOL solution_star;
        SOL solution_beta;
};

#endif