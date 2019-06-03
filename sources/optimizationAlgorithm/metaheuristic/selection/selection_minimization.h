///
/// \file selection_minimization.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2018-10
/// \brief 
///

#ifndef SELECTION_MINIMIZATION_H
#define SELECTION_MINIMIZATION_H

template<class SOL>
class Selection_minimization : public Selection<SOL> {
    public:
    Selection_minimization()  {

    }

    virtual ~Selection_minimization() {

    }

    SOL operator()(const SOL &s1, const SOL &s2) {
        assert(s1.fitnessIsValid());
        assert(s2.fitnessIsValid());
        
        if (s1.getFitness() < s2.getFitness()) {
            return s1;
        } else {
            return s2;
        }
    }
};

#endif