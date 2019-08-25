///
/// @file knapsack.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2019-05
/// @brief Knapsack Problem
///

#ifndef KNAPSACK_H
#define KNAPSACK_H

#include <map>
#include <utility>
#include <unistd.h>
#include <vector>

#include <jsoncpp/json/json.h>
#include "../solution/solutionArray.h"
#include "problem.h"


using TYPE_FITNESS_KNAPSACK = int;
using TYPE_CELL_KNAPSACK = bool;
using SOL_KNAPSACK = SolutionArray<TYPE_FITNESS_KNAPSACK, TYPE_CELL_KNAPSACK>;

class Knapsack : public Problem<SOL_KNAPSACK, TYPE_FITNESS_KNAPSACK, TYPE_CELL_KNAPSACK> {
    public:
    Knapsack() {
        
    }
    
    Knapsack(std::string fileInstance) {
        Json::Value config = loadInstance(fileInstance); 
        loadJson(config);
    }
    
    ~Knapsack() {

    }

    void loadJson(const Json::Value &config) {
        numInstance = config["problem"]["numInstance"].asString();
        capacity = config["problem"]["capacity"].asInt();
        nbItems =  config["problem"]["#items"].asUInt();

        for (unsigned int i = 0; i < nbItems ; i++) {
            weight.push_back(config["problem"]["weight"][i].asInt());
            profit.push_back(config["problem"]["profit"][i].asInt());
        }

        assert(nbItems == weight.size());
        assert(weight.size() == profit.size());
    }

    void full_eval(SOL_KNAPSACK &s) {
        int fitness = 0;
        int W = 0;

        for (unsigned int i = 0 ; i < s.sizeArray() ; i++) {
            fitness += profit[i] * s(i);
            W += weight[i] * s(i);
        }
        
		if (capacity < W)
			fitness = -1;
		s.setFitness(fitness);
    }

    /*void incremental(const SolutionArray &s, unsigned int mutatedCell) const {

    }*/

    void reset_solution(SOL_KNAPSACK &s) const {
        for (unsigned int i = 0 ; i < s.sizeArray() ; i++) {
            s(i, 0);
        }
    }

    std::unique_ptr<SOL_KNAPSACK> new_solution() const {
        std::unique_ptr<SOL_KNAPSACK> s(std::make_unique<SOL_KNAPSACK>(nbItems));
        for (unsigned int i = 0 ; i < s->sizeArray() ; i++) {
            s->operator()(i, 0);
        } 
        return std::move(s);
    }

    unsigned int sizeArraySolution() const{
        return nbItems;
    }

    TYPE_FITNESS_KNAPSACK getFitnessObjectif() const {
        return fitnessObjectif;
    }

	TYPE_FITNESS_KNAPSACK getFitnessObjectif(unsigned int numObjectif) const {
		assert(numObjectif = 0);
		return fitnessObjectif;
	}

    private:
        std::string numInstance;
        int capacity;
        unsigned int nbItems;
        std::vector<int> weight;
        std::vector<int> profit;
        int fitnessObjectif;
};

#endif