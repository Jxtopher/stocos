///
/// \file solverOneMax.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2018-10
/// \brief 
///

#ifndef SOLVERONEMAX_H
#define SOLVERONEMAX_H

#include <random>

#include <boost/program_options.hpp>

#include "solver.h"

#include "../statistic/statistic.h"
#include "../statistic/sensor.h"
#include "../statistic/sensorNumRound.h"
#include "../statistic/sensorSolution.h"
#include "../stoppingCriteria/stoppingCriteria.h"
#include "../stoppingCriteria/criteriaBudget.h"
#include "../stoppingCriteria/criteriaFitnessObjectif.h"
#include "../problem/OneMax.h"
#include "../optimizationAlgorithm/metaheuristic/firstImprovement.h"
#include "../optimizationAlgorithm/metaheuristic/bestImprovement.h"
#include "../optimizationAlgorithm/metaheuristic/onePlusLambda.h"
#include "../optimizationAlgorithm/metaheuristic/operator/mutation/flipBit.h"
#include "../optimizationAlgorithm/metaheuristic/selection/selection.h"
#include "../optimizationAlgorithm/metaheuristic/selection/selection_maximization.h"

class SolverOneMax : public Solver {
    public:
    SolverOneMax(std::mt19937 &mt_rand, 
        boost::program_options::variables_map &vm, 
        boost::program_options::options_description & desc, 
        int argc, 
        char **argv) : 
        Solver(mt_rand, vm, desc, argc, argv) {
            sizeArray= 1000;
            optimizationAlgo = 0;
            budget = 400;
            statStatistic = false;
            settings(argc, argv);

    }
    ~SolverOneMax() {

    }

    void settings(int argc, char **argv) {
        boost::program_options::options_description config("[*] OneMax parameter");
	    config.add_options()
			("N", boost::program_options::value<unsigned int>(&sizeArray), "taille de l'instance (default: 1000)")
            ("algo,A", boost::program_options::value<int>(&optimizationAlgo), "algorithme utiliser (default: )")
            ("budget", boost::program_options::value<unsigned int>(&budget), "budget alouer à l'algorithme (default:400)")
            ("statistic", boost::program_options::value<bool>(&statStatistic), "Affiche des statistiques (default:false)");
        this->_desc.add(config);

        try {
            boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(this->_desc).allow_unregistered().run(), this->_vm);
            //boost::program_options::store(boost::program_options::parse_command_line(argc, argv, _desc), this->_vm);
            boost::program_options::notify(this->_vm);
        } catch (const boost::program_options::error &ex) {
    	    std::cerr << __FILE__<<":"<<__LINE__ <<ex.what() << endl;
  	    }
        if (this->_vm.count("help")) {
                cout<<this->_desc<<endl;
                exit(EXIT_SUCCESS);
        }
    }

    void operator()() {
        SOL_ONEMAX s(1, sizeArray);
        for (unsigned int i = 0 ; i < s.sizeArray() ; i++) {
            s(i, 0);
        }
        
        this->operator()(s, optimizationAlgo);
        
        cout<<s<<endl;
    }

    void operator()(SOL_ONEMAX &s, int numParameter) {
        OneMax eOneMax(sizeArray);

        FlipBit<SOL_ONEMAX, bool> mutation_FlipBit(this->_mt_rand, 5);
        
        Selection_maximization<SOL_ONEMAX> selection;


        StoppingCriteria<SOL_ONEMAX> stoppingCriteria;
        stoppingCriteria.addCriteria(new CriteriaBudget<SOL_ONEMAX>(budget));
        stoppingCriteria.addCriteria(new CriteriaFitnessObjectif<SOL_ONEMAX>(s.sizeArray()));
        
    	Statistic<SOL_ONEMAX> statistic(statStatistic);
	    statistic.addSensor(new SensorNumRound<SOL_ONEMAX>);
	    statistic.addSensor(new SensorSolution<SOL_ONEMAX>);

        OptimizationAlgorithm<SOL_ONEMAX, bool> *optimizationAlgorithm;

        
        switch (numParameter) {
            case 0:
                optimizationAlgorithm = new FirstImprovement<SOL_ONEMAX, bool>(this->_mt_rand, statistic, stoppingCriteria, eOneMax, mutation_FlipBit, selection, sizeArray);
                optimizationAlgorithm->operator()(s);
                delete optimizationAlgorithm;
                break;
            case 1:
                optimizationAlgorithm = new BestImprovement<SOL_ONEMAX, bool>(this->_mt_rand, statistic, stoppingCriteria, eOneMax, mutation_FlipBit, selection, sizeArray);
                optimizationAlgorithm->operator()(s);
                delete optimizationAlgorithm;
                break;
            case 2:
                optimizationAlgorithm = new OnePlusLambda<SOL_ONEMAX, bool>(this->_mt_rand, statistic, stoppingCriteria, eOneMax, sizeArray, 50);
                optimizationAlgorithm->operator()(s);
                delete optimizationAlgorithm;
                break;
            default:
                cerr<<"Le choix du parameter n'est pas valide"<<endl;
                assert(false);
                break;
        }
    }
    
    //---------------
    void operator()(string &solution, int numParameter) {
        SOL_ONEMAX s(solution);
        //s.toSolution(solution);

        this->operator()(s, numParameter);

        cout<<s;
    }

    void initializationSolution() {
        OneMax eOneMax(sizeArray);
        unique_ptr<SOL_ONEMAX> s = eOneMax.new_solution();

        eOneMax.full_eval((*s));
        
        cout<<(*s);
    }

    protected:
        unsigned int sizeArray;
        int optimizationAlgo;
        unsigned int budget;
        bool statStatistic;
};

#endif