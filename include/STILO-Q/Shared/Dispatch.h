#pragma once
#include "STILO-Q/Shared/Exceptions.h"
#include "STILO-Q/Shared/Parameters.h"
#include "STILO-Q/SolverModels/GraphModels/MultiStageGraph.h"
#include "STILO-Q/SolverModels/GraphModels/SingleStageGraph.h"
#include "STILO-Q/SolverModels/GraphModels/SequentialGraph.h"
#include "STILO-Q/SolverModels/QEAModels/BinaryParticle.h"
#include "STILO-Q/SolverModels/QEAModels/MultiLevelParticle.h"

namespace QEVO
{
    template <typename Function>
    decltype(auto) dispatchBool(bool booleanValue, Function&& function)
    {
        if(booleanValue)
        {
            return function.template operator()<true>();
        }

        return function.template operator()<false>();
    }

    template <typename Function>
    decltype(auto) dispatchTourSelectionOperator(TourSelectionOperator tourSelection, Function&& function)
    {
        switch (tourSelection)
        {
        case TourSelectionOperator::EveryAnt:
            return function.template operator()<TourSelectionOperator::EveryAnt>();
        
        case TourSelectionOperator::IterationBest:
            return function.template operator()<TourSelectionOperator::IterationBest>();

        case TourSelectionOperator::GlobalBest:
            return function.template operator()<TourSelectionOperator::GlobalBest>();

        default:
            throw InvalidParameterException((char*)"Invalid enum value");
        }
    }

    template <typename Function>
    decltype(auto) dispatchPheromoneCalculationOperator(PheromoneCalculationOperator pheromoneCalculation,
        Function&& function)
    {
        if (pheromoneCalculation == PheromoneCalculationOperator::CostBased)
        {
            return function.template operator()<PheromoneCalculationOperator::CostBased>();
        }
        else if (pheromoneCalculation == PheromoneCalculationOperator::RankBased)
        {
            return function.template operator()<PheromoneCalculationOperator::RankBased>();
        }

        throw InvalidParameterException((char*)"Invalid enum value");;
    }

    template <typename Function>
    decltype(auto) dispatchPheromoneUpdateOperator(PheromoneUpdateOperator pheromoneUpdate,
        Function&& function)
    {
        if (pheromoneUpdate == PheromoneUpdateOperator::Classical)
        {
            return function.template operator()<PheromoneUpdateOperator::Classical>();
        }
        else if (pheromoneUpdate == PheromoneUpdateOperator::MaxMin)
        {
            return function.template operator()<PheromoneUpdateOperator::MaxMin>();
        }

        throw InvalidParameterException((char*)"Invalid enum value");
    }

    template <typename Function>
    decltype(auto) dispatchGraph(GraphStructure graphStructure, Function&& function)
    {
        switch (graphStructure)
        {
        case GraphStructure::MultiStage:    
            return function.template operator()<MultiStageGraph>();

        case GraphStructure::SingleStage:
            return function.template operator()<SingleStageGraph>();

        case GraphStructure::Sequential:
            return function.template operator()<SequentialGraph>(); 
        
        default:
            throw InvalidParameterException((char*)"Invalid enum value");
        }       
    }

    template <typename Function>
    decltype(auto) dispatchSelectionOperator(SelectionOperator selection, Function&& function)
    {
        switch (selection)
        {
        case SelectionOperator::Roulette:    
            return function.template operator()<SelectionOperator::Roulette>();

        case SelectionOperator::ByRank:    
            return function.template operator()<SelectionOperator::ByRank>();

        case SelectionOperator::Steady:    
            return function.template operator()<SelectionOperator::Steady>();

        case SelectionOperator::Tournament:    
            return function.template operator()<SelectionOperator::Tournament>();
        
        default:
            throw InvalidParameterException((char*)"Invalid enum value");
        }       
    }

    template <typename Function>
    decltype(auto) dispatchEvolvingParameterType(EvolvingParameterType parameterType, Function&& function)
    {
        switch (parameterType)
        {
        case EvolvingParameterType::Constant:   
            return function.template operator()<EvolvingParameterType::Constant>();

        case EvolvingParameterType::Exponential:    
            return function.template operator()<EvolvingParameterType::Exponential>();

        case EvolvingParameterType::Slow:    
            return function.template operator()<EvolvingParameterType::Slow>();

        case EvolvingParameterType::Fast:    
            return function.template operator()<EvolvingParameterType::Fast>();
        
        default:
            throw InvalidParameterException((char*)"Invalid enum value");
        }       
    }

    template <typename Function>
    decltype(auto) dispatchDistanceCalculationOperator(DistanceCalculationOperator distanceCalculation, Function&& function)
    {
        switch (distanceCalculation)
        {
        case DistanceCalculationOperator::Cauchy:
            return function.template operator()<DistanceCalculationOperator::Cauchy>();

        case DistanceCalculationOperator::Constant:    
            return function.template operator()<DistanceCalculationOperator::Constant>();

        case DistanceCalculationOperator::Gaussian:    
            return function.template operator()<DistanceCalculationOperator::Gaussian>();

        case DistanceCalculationOperator::Uniform:    
            return function.template operator()<DistanceCalculationOperator::Uniform>();
        
        default:
            throw InvalidParameterException((char*)"Invalid enum value");
        }       
    }

    template <typename Function>
    decltype(auto) dispatchParticleType(SolverType solverType, Function&& function)
    {
        switch (solverType)
        {
        case SolverType::BinaryQuantumEvolutionaryAlgorithm:
            return function.template operator()<BinaryParticle>();

        case SolverType::MultiLevelQuantumEvolutionaryAlgorithm:
            return function.template operator()<MultiLevelParticle>();
        
        default:
            throw InvalidParameterException((char*)"Solver Type must be a QEA variant");
        }
    }

    template <typename Function>
    decltype(auto) dispatchAngleCalculationOperator(AngleCalculationOperator angleCalculation, Function&& function)
    {
        switch(angleCalculation)
        {
        case AngleCalculationOperator::Constant:
            return function.template operator()<AngleCalculationOperator::Constant>();

        case AngleCalculationOperator::CostBased:
            return function.template operator()<AngleCalculationOperator::CostBased>();

        case AngleCalculationOperator::RankBased:
            return function.template operator()<AngleCalculationOperator::RankBased>();

        default:
            throw InvalidParameterException((char*)"Invalid enum value");
        }
    }

    template <typename Function>
    decltype(auto) dispatchSolutionEncoding(SolutionEncoding encoding, Function&& function)
    {
        switch (encoding)
        {
        case SolutionEncoding::Binary:
            return function.template operator()<SolutionEncoding::Binary>();

        case SolutionEncoding::Dense:
            return function.template operator()<SolutionEncoding::Dense>();

        case SolutionEncoding::Integer:
            return function.template operator()<SolutionEncoding::Integer>();

        default:
            throw InvalidParameterException((char*)"Invalid enum value");
        }
    }
}