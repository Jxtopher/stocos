///
/// @file backtracking.h
/// @author Jxtopher
/// @version 1
/// @date 2019
/// @brief *
///

#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <iostream>

#include "../../solution/solutionArray.h"
#include "../optimizationAlgorithm.h"

using namespace std;

template <typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
class Backtraking : public OptimizationAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL> {
   public:
    Backtraking(std::mt19937 &mt_rand, 
                std::unique_ptr<Statistic<SOL>> statistic, 
				std::unique_ptr<StoppingCriteria<SOL, TYPE_FITNESS>> stoppingCriteria,
                shared_ptr<Problem<SOL, TYPE_FITNESS, TYPE_CELL>> problem, 
                const unsigned int nbDigit, 
				const unsigned int len_string)
        		: OptimizationAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>(mt_rand, move(statistic), move(stoppingCriteria), problem),
          		_nbDigit(nbDigit),
                _len_string(len_string) {
                nbCall = 0;
                _string = std::unique_ptr<unsigned int[]>(new unsigned int[_len_string]);
    }

    virtual ~Backtraking() {}
    

    std::unique_ptr<SOL> operator()(const SOL &s) {
        //
        std::unique_ptr<SOL> result;
        return move(result);
    }

	void recursive(unsigned int currentCell) {
		//current_sol.print();
		if (_len_string == currentCell) {
            // for(unsigned int j = 0 ; j < _len_string ; j++)
            //     cout<<string[j];
            // cout<<endl;
			//cout<<"Wine : ";
			//current_sol.print();
		} else {
			unsigned int i = 0;
			while(i < _nbDigit) {
				_string[currentCell] = i;

				//Verification des contraites
				//if (filtering(current_sol, currentCell + 1)) {
                //if(this-> _problem.filtering(string)) {
					// Descendre dans l'arbre (parcourt en profondeur)
					recursive(currentCell + 1);
				//}
				i++;
			}
		}
	}


    string className() const {
        return "Backtraking";
    }
    
   private:
    const unsigned int _nbDigit;
    const unsigned int _len_string;

    unsigned int nbCall;
    std::unique_ptr<unsigned int[]> _string;

    const vector<unsigned int> _bijection;
};

#endif