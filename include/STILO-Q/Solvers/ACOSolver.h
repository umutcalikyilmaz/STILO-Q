#pragma once
#include "STILO-Q/Shared/Dispatch.h"
#include "STILO-Q/SolverModels/ACOModels/ACOIterator.h"
#include "STILO-Q/Solvers/Solver.h"

namespace QEVO
{
    class ACOSolver final : public Solver
    {
    public:

        SolverOutput solve(const SolverInput& input) override;

        SolverAnalysisOutput analyze(const SolverAnalysisInput& input) override;

    private:

        void checkInput(Problem* problem, const ACOConfiguration& config);

        std::unique_ptr<ACOIteratorWrapper> createIterator(GraphStructure graphStructure,
            SolutionSpaceType solutionSpaceType, PheromoneUpdateOperator pheromoneUpdate, 
            TourSelectionOperator tourSelection, PheromoneCalculationOperator pheromoneCalculation,
            Problem* problem, int stateCount, int stringLength, int antCount, int eliteCount,
            double pheromoneConstant, double pheromoneInfluence, double heuristicInfluence,
            double initialPheromone, double evaporationParameter,
            const std::vector<std::vector<std::vector<double>>>& heuristicValues,
            double localDecayParameter);
    };
}