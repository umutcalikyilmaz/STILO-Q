#pragma once
#include "STILOQ/Shared/Dispatch.h"
#include "STILOQ/Solvers/Solver.h"
#include "STILOQ/SolverModels/GAModels/Population.h"

namespace STILOQ
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