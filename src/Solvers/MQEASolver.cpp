#include "STILO-Q/Solvers/MQEASolver.h"

using namespace std::chrono_literals;

namespace QEVO
{
    SolverOutput MQEASolver::solve(const SolverInput& input)
    {
        double beginning = getTime();
        double averageTime = 0;
        double totalTime = 0;

        checkInput(input.problem, input.QEAConfig);

        std::vector<std::vector<std::vector<double>>> heuristic;

        if(input.QEAConfig.initializationType == InitializationOperator::Heuristic)
        {
            input.problem->getHeuristicValues(input.QEAConfig.graphStructure, heuristic);
        }

        auto iterator = createIterator(input.problem, input.QEAConfig.graphStructure,
            SolverType::MultiLevelQuantumEvolutionaryAlgorithm, input.QEAConfig.angleCalculationOperator,
            input.QEAConfig.globalBestRotationCount, input.QEAConfig.iterationBestRotationCount,
            input.QEAConfig.modelBestRotationCount, input.QEAConfig.populationSize, input.QEAConfig.measurementCount,
            input.QEAConfig.limitAngle, input.QEAConfig.initializationType, heuristic,
            input.QEAConfig.baseGlobalBestAngle, input.QEAConfig.baseIterationBestAngle,
            input.QEAConfig.baseModelBestAngle, input.QEAConfig.heuristicInfluence);

        int c = 0;

        while(true)
        {
            iterator->iterate(typename QEAIteratorWrapper::QEASolverKey{});
            totalTime = getTime() - beginning;
            averageTime = totalTime / ++c;
                
            if(totalTime + averageTime > input.timeLimit.count())
            {
                break;
            }
        }
    
        auto res = iterator->getOutput(typename QEAIteratorWrapper::QEASolverKey{});
        res.executionTime = totalTime * 1ms;
        iterator = nullptr;
        return res;
    }

    SolverAnalysisOutput MQEASolver::analyze(const SolverAnalysisInput& input)
    {
        double beginning = getTime();
        double totalTime = 0;

        checkInput(input.problem, input.QEAConfig);

        std::vector<std::vector<std::vector<double>>> heuristic;

        if(input.QEAConfig.initializationType == InitializationOperator::Heuristic)
        {
            input.problem->getHeuristicValues(input.QEAConfig.graphStructure, heuristic);
        }

        auto iterator = createIterator(input.problem, input.QEAConfig.graphStructure,
            SolverType::MultiLevelQuantumEvolutionaryAlgorithm, input.QEAConfig.angleCalculationOperator,
            input.QEAConfig.globalBestRotationCount, input.QEAConfig.iterationBestRotationCount,
            input.QEAConfig.modelBestRotationCount, input.QEAConfig.populationSize, input.QEAConfig.measurementCount,
            input.QEAConfig.limitAngle, input.QEAConfig.initializationType, heuristic,
            input.QEAConfig.baseGlobalBestAngle, input.QEAConfig.baseIterationBestAngle,
            input.QEAConfig.baseModelBestAngle, input.QEAConfig.heuristicInfluence);

        auto it = input.timeLimits.begin();
        SolverAnalysisOutput res;
        SolverOutput previousResult = iterator->getOutput(typename QEAIteratorWrapper::QEASolverKey{});
        std::vector<std::unique_ptr<SolverOutput>> results;
        bool exit = false;
        int c = 0;
        double wasteTime = 0;

        while(true)
        {
            iterator->iterate(typename QEAIteratorWrapper::QEASolverKey{});
            totalTime = getTime() - beginning + wasteTime;  
            
            double wasteBeg = getTime();

            while(totalTime > it->count())
            {
                if(c > 0)
                {
                    res.results[*it] = std::make_unique<SolverOutput>(previousResult);                    
                }
                else
                {
                    res.results[*it] = nullptr;
                }
                
                it++;

                if(it == input.timeLimits.end())
                {
                    exit = true;
                    break;
                }
            }

            if(exit)
            {
                break;
            }

            previousResult = iterator->getOutput(typename QEAIteratorWrapper::QEASolverKey{});
            previousResult.executionTime = totalTime * 1ms;
            c++;

            wasteTime += getTime() - wasteBeg;
        }
        
        iterator = nullptr;
        return res;
    }
}