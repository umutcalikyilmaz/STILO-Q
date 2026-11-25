#pragma once
#include <random>
#include <time.h>
#include "STILO-Q/Shared/Exceptions.h"
#include "STILO-Q/Structs/SolverStructs/SolverInput.h"
#include "STILO-Q/Structs/SolverStructs/SolverAnalysisInput.h"
#include "STILO-Q/Structs/SolverStructs/SolverAnalysisOutput.h"

namespace QEVO
{
    class Solver
    {
    public:

        virtual SolverOutput solve(const SolverInput& inp) = 0;

        virtual SolverAnalysisOutput analyze(const SolverAnalysisInput& inp) = 0;

    protected:
        
        Problem* prb;

        double getTime();
    };
}