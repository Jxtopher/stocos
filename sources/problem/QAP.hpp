///
/// @file QAP.hpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2019-10
/// @brief Quadratic Assignment Problem
///
///             ---  ---
///             |    |
///      sol =  /    /    a    b         .
///             ---  ---   ij   p(i),p(j)
///              i    j
///         see : https://www.opt.math.tugraz.at/qaplib/inst.html
///         see : http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.94.9558&rep=rep1&type=pdf
///

#ifndef EVALQAP_H
#define EVALQAP_H

#include <iostream>
#include <memory>

namespace stocos {

using TYPE_FITNESS_QAP = unsigned int;
using TYPE_CELL_QAP = unsigned int;
using SOL_QAP = SolutionArray<TYPE_FITNESS_QAP, TYPE_CELL_QAP>;
class QAP : public Problem<SOL_QAP, TYPE_FITNESS_QAP, TYPE_CELL_QAP> {
  public:
	QAP(const std::string& fileInstance) {
		Json::Value config = loadInstance(fileInstance);
		loadJson(config);
	}

	QAP(unsigned int _n, unsigned int** _A, unsigned int** _B) : n(_n) {
		// Allocation of A and B
		A = new unsigned int*[n];
		B = new unsigned int*[n];
		for(unsigned int i = 0; i < n; i++) {
			A[i] = new unsigned int[n];
			B[i] = new unsigned int[n];
		}

		// Copy
		for(unsigned int i = 0; i < n; i++) {
			for(unsigned int j = 0; j < n; j++) {
				A[i][j] = _A[i][j];
				B[i][j] = _B[i][j];
			}
		}
	}

	// virtual ~QAP() {
	// 	for(unsigned int i = 0; i < n; i++) {
	// 		delete[] A[i];
	// 		delete[] B[i];
	// 	}
	// 	delete[] A;
	// 	delete[] B;
	// }

	// void loadInstance(const std::string &file) {
	// 	std::fstream myfile(file, std::ios_base::in);

	// 	unsigned int a;
	// 	myfile >> n;
	// 	// Allocation of A and B
	// 	A = new unsigned int *[n];
	// 	B = new unsigned int *[n];
	// 	for (unsigned int i=0; i < n; i++){
	// 		A[i] = new unsigned int[n];
	// 		B[i] = new unsigned int[n];
	// 	}

	// 	unsigned int i = 0, j = 0;
	// 	while (myfile >> a) {
	// 		A[i][j] = a;
	// 		if (n - 1 <= i && n - 1 <= j)
	// 			break;
	// 		else if (n - 1 <= j) {
	// 			i++;
	// 			j = 0;
	// 		} else
	// 			j++;
	// 	}

	// 	i = 0, j = 0;
	// 	while (myfile >> a) {
	// 		B[i][j] = a;
	// 		if (n - 1 <= i && n - 1 <= j)
	// 			break;
	// 		else if (n - 1 <= j) {
	// 			i++;
	// 			j = 0;
	// 		} else
	// 			j++;
	// 	}
	// }
	void loadJson(const Json::Value& config) {
	}

	void evaluation(SOL_QAP& p) {
		unsigned int sum = 0;
		for(unsigned int i = 0; i < n; i++) {
			for(unsigned int j = 0; j < n; j++) sum += A[i][j] * B[p(i) - 1][p(j) - 1];
		}
		p.setFitness(0, sum);
	}

	void resetSolution(SOL_QAP& s) const {
	}

	void showA() const {
		for(unsigned int i = 0; i < n; i++) {
			for(unsigned int j = 0; j < n; j++) std::cout << A[i][j] << "\t";
			std::cout << std::endl;
		}
		std::cout << std::endl;
		for(unsigned int i = 0; i < n; i++) {
			for(unsigned int j = 0; j < n; j++) std::cout << B[i][j] << "\t";
			std::cout << std::endl;
		}
	}

  private:
	unsigned int n;   // instance size
	unsigned int** A; // distance between locations i and j
	unsigned int** B; // the flow between facilities r and s
};

} // namespace stocos
#endif
