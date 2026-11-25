#pragma once
#include "STILOQ/Shared/Dispatch.h"
#include "STILOQ/Solvers/Solver.h"
#include "STILOQ/SolverModels/SAModels/SAIterator.h"

namespace STILOQ
{
    class SASolver final : public Solver
    {
    public:

        SolverOutput solve(const SolverInput& input) override;

        SolverAnalysisOutput analyze(const SolverAnalysisInput& input) override;

    private:

        void checkInput(Problem* problem, const SAConfiguration& config);

        std::unique_ptr<SAIteratorWrapper> createIterator(EvolvingParameterType cooling,
            DistanceCalculationOperator distanceCalculation, Problem* problem,
            int initialTemperature, int minimumNeighorhoodSize, int maximumNeighorhoodSize,
            double coolingParameter, double pointMoveCoefficient, double insertMoveCoefficient,
            double invertMoveCoefficient, double swapMoveCoefficient);
    };
}