#pragma once
#include "STILOQ/Shared/Dispatch.h"
#include "STILOQ/SolverModels/QEAModels/QEAIterator.h"
#include "STILOQ/Solvers/QEASolver.h"

namespace STILOQ
{
    class BQEASolver final : public QEASolver
    {
    public:
    
        SolverOutput solve(const SolverInput& input) override;

        SolverAnalysisOutput analyze(const SolverAnalysisInput& input) override;

    private:

        void checkBinary(Problem* problem, const QEAConfiguration& config);
    };
}