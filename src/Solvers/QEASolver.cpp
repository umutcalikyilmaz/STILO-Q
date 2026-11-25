#include "STILOQ/Solvers/QEASolver.h"

namespace STILOQ
{
    void QEASolver::checkInput(Problem* problem, const QEAConfiguration& config)
    {
        if(config.globalBestRotationCount < 0)
        {
            throw InvalidParameterException((char*)"Global rotation count cannot be negative");
        }

        if(config.iterationBestRotationCount < 0)
        {
            throw InvalidParameterException((char*)"Iteration rotation count cannot be negative");
        }

        if(config.modelBestRotationCount < 0)
        {
            throw InvalidParameterException((char*)"Local rotation count cannot be negative");
        }

        if(config.populationSize <= 0)
        {
            throw InvalidParameterException((char*)"Population size must be greater than 0");
        }

        if(config.measurementCount <= 0)
        {
            throw InvalidParameterException((char*)"Measurement count must be greater than 0");
        }

        if(config.globalBestRotationCount > 0 && config.baseGlobalBestAngle <= 0)
        {
            throw InvalidParameterException((char*)"Initial global angle must be greater than 0");
        }

        if(config.iterationBestRotationCount > 0 && config.baseIterationBestAngle <= 0)
        {
            throw InvalidParameterException((char*)"Initial iteration angle must be greater than 0");
        }

        if(config.modelBestRotationCount > 0 && config.baseModelBestAngle <= 0)
        {
            throw InvalidParameterException((char*)"Initial local angle must be greater than 0");
        }

        if(config.globalBestRotationCount == 0 && config.iterationBestRotationCount == 0 && config.modelBestRotationCount == 0)
        {
            throw InvalidParameterException((char*)"Number of global, iteration, and local rotations cannot all be set to 0");
        }

        if(config.limitAngle < 0 || config.limitAngle > M_PI_4)
        {
            throw InvalidParameterException((char*)"Limit angle value must be in range [0, pi/4]");
        }
    }

    std::unique_ptr<QEAIteratorWrapper> QEASolver::createIterator(Problem* problem, GraphStructure graphStructure,
        SolverType solverType, AngleCalculationOperator angleCalculation, int globalBestRotationCount,
        int iterationBestRotationCount, int modelBestRotationCount, int populationSize, int measurementCount,
        double limitAngle, InitializationOperator initialization,
        std::vector<std::vector<std::vector<double>>> heuristic, double baseGlobalBestAngle,
        double baseIterationBestAngle, double baseModelBestAngle, double heuristicInfluence)
    {
        int stateCount = problem->getStateCount();
        bool isUnique = problem->getSolutionSpaceType() == SolutionSpaceType::Permutation;
        bool globalBestRotation = globalBestRotationCount > 0;
        bool iterationBestRotation = iterationBestRotationCount > 0;
        bool modelBestRotation = modelBestRotationCount > 0;
        SolverType finalSolverType = solverType;
        SolutionEncoding encoding;

        if(solverType == SolverType::MultiLevelQuantumEvolutionaryAlgorithm)
        {
            encoding = SolutionEncoding::Integer;
        }

        if(stateCount == 2)
        {
            encoding = SolutionEncoding::Integer;
            finalSolverType = SolverType::BinaryQuantumEvolutionaryAlgorithm;
        }
        else
        {
            if(solverType == SolverType::BinaryQuantumEvolutionaryAlgorithm)
            {
                if(isUnique)
                {
                    isUnique = false;
                    encoding = SolutionEncoding::Dense;
                }
                else
                {
                    encoding = SolutionEncoding::Binary;
                }
            }
        }

        return dispatchGraph(graphStructure, [&]<template <class, bool> class kGraphType>() -> std::unique_ptr<QEAIteratorWrapper>
        {
            return dispatchParticleType(finalSolverType, [&]<class kParticleType>() -> std::unique_ptr<QEAIteratorWrapper>
            {
                return dispatchBool(isUnique, [&]<bool kIsUnique>() -> std::unique_ptr<QEAIteratorWrapper>
                {
                    return dispatchSolutionEncoding(encoding, [&]<SolutionEncoding kEncoding>() -> std::unique_ptr<QEAIteratorWrapper>
                    {
                        return dispatchAngleCalculationOperator(angleCalculation, [&]<AngleCalculationOperator kAngleCalculation>() -> std::unique_ptr<QEAIteratorWrapper>
                        {                                
                            return dispatchBool(globalBestRotation, [&]<bool kGlobalBestRotation>() -> std::unique_ptr<QEAIteratorWrapper>
                            {          
                                return dispatchBool(iterationBestRotation, [&]<bool kIterationBestRotation>() -> std::unique_ptr<QEAIteratorWrapper>
                                {
                                    return dispatchBool(modelBestRotation, [&]<bool kModelBestRotation>() -> std::unique_ptr<QEAIteratorWrapper>
                                    {
                                        return std::make_unique<QEAIterator<kGraphType, kParticleType, kIsUnique, kEncoding, kAngleCalculation, kGlobalBestRotation, kIterationBestRotation, kModelBestRotation>>(
                                            typename QEAIteratorWrapper::QEASolverKey{}, problem, populationSize, measurementCount, globalBestRotationCount,
                                            iterationBestRotationCount, modelBestRotationCount, limitAngle, initialization, heuristic, baseGlobalBestAngle,
                                            baseIterationBestAngle, baseModelBestAngle, heuristicInfluence);
                                    }); 
                                });                          
                            });                                
                        });
                    });
                });
            });
        });
    }
}