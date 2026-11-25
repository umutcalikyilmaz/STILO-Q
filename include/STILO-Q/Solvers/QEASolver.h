#pragma once
#include "STILO-Q/Shared/Dispatch.h"
#include "STILO-Q/SolverModels/QEAModels/QEAIterator.h"
#include "STILO-Q/Solvers/Solver.h"

namespace QEVO
{
    class QEASolver : public Solver
    {
    public:
        virtual SolverOutput solve(const SolverInput& input) = 0;

        virtual SolverAnalysisOutput analyze(const SolverAnalysisInput& input) = 0;

    protected:

        void checkInput(Problem* problem, const QEAConfiguration& config);

        std::unique_ptr<QEAIteratorWrapper> createIterator(Problem* problem, GraphStructure graphStructure,
            SolverType solverType, AngleCalculationOperator angleCalculation, int globalBestRotationCount,
            int iterationBestRotationCount, int modelBestRotationCount, int populationSize, int measurementCount,
            double limitAngle, InitializationOperator initialization,
            std::vector<std::vector<std::vector<double>>> heuristic, double baseGlobalBestAngle,
            double baseIterationBestAngle, double baseModelBestAngle, double heuristicInfluence);
    };
}