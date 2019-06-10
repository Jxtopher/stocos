///
/// \file simulatedAnnealing.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2018-10
/// \brief 
///

#ifndef SIMULATEDANNELING_H
#define SIMULATEDANNELING_H


template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
class SimulatedAnnealing : public OptimizationAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL> {
    public:
    SimulatedAnnealing(std::mt19937 &mt_rand, 
        Statistic<SOL> &statistic,
        StoppingCriteria<SOL, TYPE_FITNESS> &stoppingCriteria,
        Problem<SOL, TYPE_FITNESS, TYPE_CELL> &problem,
        AtomicOperation<SOL, TYPE_FITNESS, TYPE_CELL> &atomicOperations,
        Selection<SOL> &selection) : 
        OptimizationAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>(mt_rand, statistic, stoppingCriteria, problem),
        _atomicOperations(atomicOperations),
        _selection(selection) {
        DEBUG_TRACE("Creation SimulatedAnnealing");
    }
    virtual ~SimulatedAnnealing() {
        
    }

    unique_ptr<SOL> operator()(const SOL &s) {
        solution_star = s;
        if (!solution_star.fitnessIsValid()) {
            this->_problem.full_eval(solution_star);
        }

        return move(make_unique<SOL>(solution_star));
    }

    protected:
    AtomicOperation<SOL, TYPE_FITNESS, TYPE_CELL> &_atomicOperations;
    Selection<SOL> &_selection;

    SOL solution_star;
};

#endif