#pragma once
#include <cstdint>

using namespace std;

namespace STILOQ
{
    enum class ProbabilityDistribution : uint8_t
	{
		Uniform, 
		Normal
	};

    enum class GraphStructure : uint8_t
    {
        MultiStage,
        SingleStage,
        Sequential
    };

    enum class EvolvingParameterType : uint8_t
    {
        Constant,
        Exponential,
        Slow,
        Fast
    };

    enum class AngleEvolutionOperator : uint8_t
    {
        Constant,
        Exponential,
        Linear,
        Logarithmic,
    };

    enum class PheromoneUpdateOperator : uint8_t
    {
        Classical,
        MaxMin,
    };

    enum class PheromoneCalculationOperator : uint8_t
    {
        CostBased,
        RankBased
    };

    enum class TourSelectionOperator : uint8_t
    {
        EveryAnt,
        IterationBest,
        GlobalBest
    };

    enum class CrossoverOperator : uint8_t
    {
        Cycle,
        Kpoint,        
        OX,
        PMX,
        Uniform
    };

    enum class MutationOperator : uint8_t
    {
        Inversion,
        Point,
        Scramble,
        Swap
    };

    enum class SelectionOperator : uint8_t
    {        
        Roulette,
        ByRank,
        Steady,
        Tournament
    };

    enum class SolverType : uint8_t
    {
        AntColonyOptimization,
        GeneticAlgorithm,
        BinaryQuantumEvolutionaryAlgorithm,
        MultiLevelQuantumEvolutionaryAlgorithm,
        SimulatedAnnealing
    };

    enum class ProblemType : uint8_t
    {
        GraphColoring,
        IdenticalMachinesScheduling,
        Knapsack,
        MaxCut,
        TransactionScheduling,
        TravelingSalesperson,
        UnrelatedMachinesScheduling,
        WeightedMaxCut
    };

    enum class SolutionSpaceType : uint8_t
    {
        Permutation,
        Vector
    };

    enum class AngleCalculationOperator : uint8_t
    {       
        Constant, 
        CostBased,
        RankBased
    };

    enum class InitializationOperator : uint8_t
    {
        Heuristic,
        Standard
    };

    enum class MoveOperator : uint8_t
    {
        Point,        
        Insert,
        Invert,
        Swap
    };

    enum class DistanceCalculationOperator : uint8_t
    {
        Cauchy,
        Constant,
        Gaussian,        
        Uniform
    };

    enum class SelectionCriterion : uint8_t
    {
        Minimum,
        Maximum,
        Average,
        Median,
        StdDeviation
    };

    enum class SolutionEncoding : uint8_t
    {
        Binary,
        Dense,
        Integer
    };
}