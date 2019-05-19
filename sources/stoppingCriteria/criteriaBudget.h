///
/// \file criteriaBudget.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2018-10
/// \brief 
///

#ifndef CRITERIABUDGET_H
#define CRITERIABUDGET_H

#include "criteria.h"

template<class SOL>
class CriteriaBudget : public Criteria<SOL> {
    public:
    CriteriaBudget(unsigned int _budget) : 
        Criteria<SOL>(), 
        budget(_budget) {
        consumedBudget = 0;
    }

    virtual ~CriteriaBudget(){

    }

    virtual bool operator()(const SOL &s) {
        return consumedBudget++ < budget;
    }

    unsigned int getConsumedBudget() const {
        return consumedBudget;
    }

    void resetConsumedBudget() {
        consumedBudget = 0;
    }

    protected:
    unsigned int consumedBudget;
    unsigned int budget;
};

#endif