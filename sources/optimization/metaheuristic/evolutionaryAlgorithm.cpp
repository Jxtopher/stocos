///
/// @file evolutionaryAlgorithm.hpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2019-06
/// @brief
///
#include <optimization/metaheuristic/evolutionaryAlgorithm.h>
namespace stocos {
template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
EvolutionaryAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>::EvolutionaryAlgorithm(
    std::mt19937& mt_rand, std::shared_ptr<Statistic<SOL>> statistic,
    std::unique_ptr<StoppingCriteria<SOL, TYPE_FITNESS>> stoppingCriteria,
    std::shared_ptr<Problem<SOL, TYPE_FITNESS, TYPE_CELL>> problem,
    std::unique_ptr<AtomicOperation<SOL, TYPE_FITNESS, TYPE_CELL>> atomicOperations,
    unsigned int mu, unsigned int lambda)
    : Optimization<SOL, TYPE_FITNESS, TYPE_CELL>(mt_rand, std::move(statistic),
                                                 move(stoppingCriteria), problem),
      _atomicOperations(move(atomicOperations)), _mu(mu), _lambda(lambda) {
    BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " Creation EvolutionaryAlgorithm";
    assert(_mu != 0);
    rid_parents = std::make_unique<std::uniform_int_distribution<unsigned int>>(0, _mu - 1);
    rid_offsprings = std::make_unique<std::uniform_int_distribution<unsigned int>>(0, _lambda - 1);
}

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
EvolutionaryAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>::~EvolutionaryAlgorithm() {
}

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
std::unique_ptr<SOL> EvolutionaryAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>::operator()(const SOL& s) {
    solution_star = s;

    if(!solution_star.fitnessIsValid()) { this->_problem->evaluation(solution_star); }

    // Initialisation de la population à partir d'une solution
    for(SOL& p : parents) p = solution_star;

    while(parents.size() < _mu) parents.push_back(solution_star);

    for(SOL& p : parents) _atomicOperations->operator()(p);

    // Evaluation des parents
    for(SOL& p : parents) this->_problem->evaluation(p);

    while(offsprings.size() < _lambda) offsprings.push_back(solution_star);

    //
    while(this->_stoppingCriteria->operator()(solution_star)) {
        this->_statistic->operator()(solution_star, className());

        // selection de deux parents aléatoire et différent pour construire la population
        // enfants
        for(auto it = offsprings.begin(); it != offsprings.end(); ++it) {
            unsigned int e1 = rid_parents->operator()(this->_mt_rand);
            unsigned int e2 = rid_parents->operator()(this->_mt_rand);

            if(_mu != 1)
                while(e1 == e2) e2 = rid_parents->operator()(this->_mt_rand);

            if(it == offsprings.begin()) {
                *it = solution_star; // * Ré-introduction de la meilleurs solution dans la
                                     // population
            } else {
                if(this->_problem->solutionSelection(parents[e1], parents[e2]))
                    *it = parents[e1];
                else
                    *it = parents[e2];
            }
        }

        // Mutation des enfants et construction la population parents.
        for(SOL& o : offsprings) _atomicOperations->operator()(o);

        // Remplacement
        if(_mu == _lambda) {
            for(unsigned int i = 0; i < _mu; i++) parents[i] = offsprings[i];
        } else {
            for(unsigned int i = 0; i < _mu; i++)
                parents[i] = offsprings[rid_offsprings->operator()(this->_mt_rand)];
        }

        // Evaluation des parents
        for(SOL& p : parents) this->_problem->evaluation(p);

        // Find best
        for(SOL& p : parents) {
            if(this->_problem->solutionSelection(p, solution_star)) solution_star = p;
        }
    }

    return std::move(std::make_unique<SOL>(solution_star));
}

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
std::string EvolutionaryAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>::className() const {
    if(_class_name.empty())
        return "EvolutionaryAlgorithm";
    else
        return _class_name;
}

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
void EvolutionaryAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>::className(const std::string& class_name) {
    _class_name = class_name;
}

template class EvolutionaryAlgorithm<Solution<double, double>, double, double>;
template class EvolutionaryAlgorithm<Solution<double, bool>, double, bool>;
template class EvolutionaryAlgorithm<Solution<int, bool>, int, bool>;
template class EvolutionaryAlgorithm<Solution<unsigned int, bool>, unsigned int, bool>;
template class EvolutionaryAlgorithm<Solution<double, unsigned int>, double, unsigned int>;
template class EvolutionaryAlgorithm<Solution<unsigned int, unsigned int>, unsigned int,
                                     unsigned int>;
} // namespace stocos
