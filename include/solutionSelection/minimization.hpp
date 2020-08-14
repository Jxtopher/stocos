///
/// @file selection_minimization.hpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief
///

#ifndef MINIMIZATION_H
#define MINIMIZATION_H

#include "solutionSelection.hpp"

namespace stocos {

template<class SOL>
class Minimization : public SolutionSelection<SOL> {
  public:
	Minimization() {
	}

	virtual ~Minimization() {
	}

	bool operator()(const SOL& s1, const SOL& s2, const unsigned int numObjectif = 0) const {
		assert(s1.fitnessIsValid(numObjectif));
		assert(s2.fitnessIsValid(numObjectif));

		unsigned int count = 0;
		unsigned int count_equal = 0;
		for(unsigned int i = 0; i < s1.numberOfObjective(); i++) {
			if(s1.getFitness(i) < s2.getFitness(i))
				count++;
			else if(s2.getFitness(i) == s1.getFitness(i)) {
				count++;
				count_equal++;
			}
		}

		return count == s1.numberOfObjective() && count_equal != s1.numberOfObjective();
	}

	unsigned int operator()(const Population<SOL>& p) const {
		assert(0 < p.size());

		double fitness = p[0].getFitness();
		unsigned int index = 0;
		for(unsigned int i = 0; i < p.size(); i++) {
			if(p[i].getFitness() < fitness) {
				fitness = p[i].getFitness();
				index = i;
			}
		}
		return index;
	}
};

} // namespace stocos

#endif