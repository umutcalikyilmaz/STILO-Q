#pragma once
#include "STILO-Q/Shared/Dispatch.h"
#include "STILO-Q/SolverModels/QEAModels/QEAIterator.h"
#include "STILO-Q/Solvers/QEASolver.h"

namespace QEVO
{
    class MQEASolver final : public QEASolver
    {
    public:
        
        SolverOutput solve(const SolverInput& input) override;

        SolverAnalysisOutput analyze(const SolverAnalysisInput& input) override;
    };
}