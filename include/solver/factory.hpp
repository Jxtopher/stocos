///
/// @file factory.hpp
/// @author Jxtopher
/// @brief
/// @version 0.1
/// @date 2019-08-30
///
///

#ifndef ALGOBUILDER_H
#define ALGOBUILDER_H

#include <map>
#include <memory>
#include <random>

#include "../optimization/exhaustiveSearch/backtracking.hpp"
#include "../optimization/exhaustiveSearch/combinationGenerator.hpp"
#include "../optimization/metaheuristic/bestImprovement.hpp"
#include "../optimization/metaheuristic/evolutionaryAlgorithm.hpp"
#include "../optimization/metaheuristic/firstImprovement.hpp"
#include "../optimization/metaheuristic/iteratedLocalSearch.hpp"
#include "../optimization/metaheuristic/onePlusLambda.hpp"
#include "../optimization/metaheuristic/operator/mutation/flipBit.hpp"
#include "../optimization/metaheuristic/operator/mutation/intervalReal.hpp"
#include "../optimization/metaheuristic/operator/mutation/neighborhood.hpp"
#include "../optimization/metaheuristic/operator/shuffle.hpp"
#include "../optimization/metaheuristic/operator/swap.hpp"
#include "../optimization/metaheuristic/simulatedAnnealing.hpp"
#include "../optimization/metaheuristic/tabuSearch.hpp"
#include "../optimization/nearestNeighbourAlgorithm.hpp"
#include "../problem/problem.hpp"
#include "../statistic/sensor.hpp"
#include "../statistic/sensorExperience.hpp"
#include "../statistic/sensorFinal.hpp"
#include "../statistic/sensorFitness.hpp"
#include "../statistic/sensorNameCallingClass.hpp"
#include "../statistic/sensorNumRound.hpp"
#include "../statistic/sensorSolution.hpp"
#include "../statistic/sensorStopwatch.hpp"
#include "../statistic/statistic.hpp"
#include "../stoppingCriteria/criteriaBudget.hpp"
#include "../stoppingCriteria/criteriaFitnessObjectif.hpp"
#include "../stoppingCriteria/stoppingCriteria.hpp"

namespace stocos {

template<typename SOL, typename TYPE_FITNESS, typename TYPE_CELL>
class Factory {
  public:
	Factory(std::mt19937& mt_rand, std::shared_ptr<Problem<SOL, TYPE_FITNESS, TYPE_CELL>> problem,
			const Json::Value& configuration)
		: _mt_rand(mt_rand), _problem(problem) {
		_statistic = statistic(configuration["Statistic"]);
	}

	virtual ~Factory() {
	}

	std::unique_ptr<OptimizationAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>>
		operator()(const Json::Value& configuration) {
		std::unique_ptr<OptimizationAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>> optimizationAlgorithm =
			nullptr;
		std::unique_ptr<StoppingCriteria<SOL, TYPE_FITNESS>> _stoppingCriteria =
			stoppingCriteria(configuration["StoppingCriteria"]);
		std::unique_ptr<AtomicOperation<SOL, TYPE_FITNESS, TYPE_CELL>> _atomicOperation = nullptr;

		if(configuration["className"] != "NearestNeighbourAlgorithm" &&
		   configuration["className"] != "CombinationGenerator")
			_atomicOperation = atomicOperation(configuration["AtomicOperation"]);

		if(configuration["className"] == "FirstImprovement") {
			optimizationAlgorithm =
				std::make_unique<FirstImprovement<SOL, TYPE_FITNESS, TYPE_CELL>>(
					this->_mt_rand, _statistic, std::move(_stoppingCriteria), _problem,
					std::move(_atomicOperation));
		} else if(configuration["className"] == "BestImprovement") {
			// optimizationAlgorithm = std::make_unique<BestImprovement<SOL, TYPE_FITNESS,
			// TYPE_CELL>>(this->_mt_rand, *_statistic, *_stoppingCriteria, _problem,
			// *_atomicOperation, *_selection);
		} else if(configuration["className"] == "OnePlusLambda") {
			optimizationAlgorithm = std::make_unique<OnePlusLambda<SOL, TYPE_FITNESS, TYPE_CELL>>(
				this->_mt_rand, _statistic, std::move(_stoppingCriteria), _problem,
				std::move(_atomicOperation), configuration["lambda"].asUInt());
		} else if(configuration["className"] == "IteratedLocalSearch") {
			std::unique_ptr<OptimizationAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>>
				_optimizationAlgorithm_ils =
					this->operator()(configuration["OptimizationAlgorithm"]);
			optimizationAlgorithm =
				std::make_unique<IteratedLocalSearch<SOL, TYPE_FITNESS, TYPE_CELL>>(
					this->_mt_rand, _statistic, std::move(_stoppingCriteria), _problem,
					std::move(_atomicOperation), std::move(_optimizationAlgorithm_ils));
		} else if(configuration["className"] == "TabuSearch") {
			optimizationAlgorithm = std::make_unique<TabuSearch<SOL, TYPE_FITNESS, TYPE_CELL>>(
				this->_mt_rand, _statistic, std::move(_stoppingCriteria), _problem,
				std::move(_atomicOperation));
		} else if(configuration["className"] == "EvolutionaryAlgorithm") {
			optimizationAlgorithm =
				std::make_unique<EvolutionaryAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>>(
					this->_mt_rand, _statistic, std::move(_stoppingCriteria), _problem,
					std::move(_atomicOperation), configuration["mu"].asUInt(),
					configuration["lambda"].asUInt());
		} else if(configuration["className"] == "OnePlusLambda") {
			optimizationAlgorithm = std::make_unique<OnePlusLambda<SOL, TYPE_FITNESS, TYPE_CELL>>(
				this->_mt_rand, _statistic, std::move(_stoppingCriteria), _problem,
				std::move(_atomicOperation), configuration["lambda"].asInt());
		} else if(configuration["className"] == "SimulatedAnnealing") {
			optimizationAlgorithm =
				std::make_unique<SimulatedAnnealing<SOL, TYPE_FITNESS, TYPE_CELL>>(
					this->_mt_rand, _statistic, std::move(_stoppingCriteria), _problem,
					std::move(_atomicOperation));
		} else if(configuration["className"] == "Backtraking") {
		} else if(configuration["className"] == "CombinationGenerator") {
			optimizationAlgorithm =
				std::make_unique<CombinationGenerator<SOL, TYPE_FITNESS, TYPE_CELL>>(
					this->_mt_rand, _statistic, std::move(_stoppingCriteria), _problem);
		} else if(configuration["className"] == "NearestNeighbourAlgorithm") {
			optimizationAlgorithm =
				std::make_unique<NearestNeighbourAlgorithm<SOL, TYPE_FITNESS, TYPE_CELL>>(
					this->_mt_rand, _statistic, std::move(_stoppingCriteria), _problem);
		} else {
			throw std::runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) +
									 " The algorithm " + configuration["className"].asString() +
									 " does not exist.");
		}

		return std::move(optimizationAlgorithm);
	}

	void sensor(const Json::Value& configuration, std::shared_ptr<Statistic<SOL>> __statistic) {
		if(!configuration["sensorExperience"].empty()) {
			__statistic->addSensor(
				new SensorExperience<SOL>(configuration["sensorExperience"]["num"].asUInt(),
										  configuration["sensorExperience"]["name"].asString()));
		}
		if(!configuration["sensorNumRound"].empty()) {
			if(configuration["sensorNumRound"] == true)
				__statistic->addSensor(new SensorNumRound<SOL>);
		}
		if(!configuration["sensorSolution"].empty()) {
			if(configuration["sensorSolution"] == true)
				__statistic->addSensor(new SensorSolution<SOL>);
		}
		if(!configuration["sensorFitness"].empty()) {
			if(configuration["sensorFitness"] == true)
				__statistic->addSensor(new SensorFitness<SOL>);
		}
		if(!configuration["sensorStopwatch"].empty()) {
			if(configuration["sensorStopwatch"] == true)
				__statistic->addSensor(new SensorStopwatch<SOL>);
		}
		if(!configuration["sensorNameCallingClass"].empty()) {
			if(configuration["sensorNameCallingClass"] == true)
				__statistic->addSensor(new SensorNameCallingClass<SOL>);
		}
		if(!configuration["sensorFinal"].empty()) {
			__statistic->addSensor(
				new SensorFinal<SOL>(configuration["sensorFinal"]["num"].asUInt(),
									 configuration["sensorFinal"]["name"].asString()));
		}
	}

	std::shared_ptr<Statistic<SOL>> getStatistic() {
		return _statistic;
	}

	std::unique_ptr<StoppingCriteria<SOL, TYPE_FITNESS>>
		stoppingCriteria(const Json::Value& configuration) {
		std::unique_ptr<StoppingCriteria<SOL, TYPE_FITNESS>> _stoppingCriteria =
			std::make_unique<StoppingCriteria<SOL, TYPE_FITNESS>>();
		if(!configuration["budget"].empty())
			_stoppingCriteria->addCriteria(
				new CriteriaBudget<SOL, TYPE_FITNESS>(configuration["budget"].asUInt()));
		if(!configuration["fitnessObjectif"].empty())
			_stoppingCriteria->addCriteria(new CriteriaFitnessObjectif<SOL, TYPE_FITNESS>(
				static_cast<TYPE_FITNESS>(configuration["fitnessObjectif"].asDouble())));
		return std::move(_stoppingCriteria);
	}

	std::unique_ptr<AtomicOperation<SOL, TYPE_FITNESS, TYPE_CELL>>
		atomicOperation(const Json::Value& configuration) {
		if(configuration["className"].asString() == "FlipBit") {
			return std::make_unique<FlipBit<SOL, TYPE_FITNESS, TYPE_CELL>>(
				this->_mt_rand, _problem, configuration["c"].asInt());
		} else if(configuration["className"].asString() == "IntervalReal") {
			return std::make_unique<IntervalReal<SOL, TYPE_FITNESS, TYPE_CELL>>(
				this->_mt_rand, _problem, configuration["c"].asInt(), configuration["a"].asInt(),
				configuration["b"].asInt());
		} else if(configuration["className"].asString() == "Neighborhood") {
			// return std::make_unique<Neighborhood<SOL, TYPE_FITNESS, SOL>>(this->_mt_rand);
			// throw std::runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) +
			// " Not implemented : "+configuration["className"].asString());
			return nullptr;
		} else if(configuration["className"].asString() == "Swap") {
			return std::make_unique<Swap<SOL, TYPE_FITNESS, TYPE_CELL>>(
				this->_mt_rand, _problem, configuration["number_of_swap"].asInt());
		} else if(configuration["className"].asString() == "Shuffle") {
			return std::make_unique<Shuffle<SOL, TYPE_FITNESS, TYPE_CELL>>(this->_mt_rand,
																		   _problem);
		} else
			throw std::runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) +
									 " The atomicOperation " +
									 configuration["className"].asString() + " does not exist.");
		return nullptr;
	}

  private:
	std::mt19937& _mt_rand;
	std::shared_ptr<Problem<SOL, TYPE_FITNESS, TYPE_CELL>> _problem;
	std::shared_ptr<Statistic<SOL>> _statistic;

	std::shared_ptr<Statistic<SOL>> statistic(const Json::Value& configuration) {
		std::shared_ptr<Statistic<SOL>> __statistic;
		if(Statistic<SOL>::NONE == configuration["recording"].asString()) {
			__statistic = std::make_shared<Statistic<SOL>>(true);
		} else if(Statistic<SOL>::STDOUT == configuration["recording"].asString()) {
			__statistic = std::make_shared<Statistic<SOL>>(false);
			sensor(configuration, __statistic);
		} else if(Statistic<SOL>::FILE == configuration["recording"].asString()) {
			__statistic = std::make_shared<Statistic<SOL>>(configuration["namefile"].asString());
			sensor(configuration, __statistic);
		} else if(Statistic<SOL>::MONGODB == configuration["recording"].asString()) {
			__statistic = std::make_shared<Statistic<SOL>>(configuration["hostname"].asString(),
														   configuration["database"].asString(),
														   configuration["collection"].asString());
			sensor(configuration, __statistic);
		} else {
			throw std::runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) +
									 " The recording statistic " +
									 configuration["recording"].asString() + " does not exist.");
		}
		return std::move(__statistic);
	}
};

} // namespace stocos

#endif