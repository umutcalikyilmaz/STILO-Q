#pragma once
#include "STILO-Q/Shared/Dispatch.h"
#include "STILO-Q/Solvers/Solver.h"
#include "STILO-Q/SolverModels/GAModels/Population.h"

namespace QEVO
{
    class GASolver final : public Solver
    {
    public:

        SolverOutput solve(const SolverInput& input) override;

        SolverAnalysisOutput analyze(const SolverAnalysisInput& input) override;

    private:

        void checkInput(Problem* problem, const GAConfiguration& config);
    };
}