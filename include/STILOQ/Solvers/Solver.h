#pragma once
#include <random>
#include <time.h>
#include "STILOQ/Shared/Exceptions.h"
#include "STILOQ/Structs/SolverStructs/SolverInput.h"
#include "STILOQ/Structs/SolverStructs/SolverAnalysisInput.h"
#include "STILOQ/Structs/SolverStructs/SolverAnalysisOutput.h"

namespace STILOQ
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