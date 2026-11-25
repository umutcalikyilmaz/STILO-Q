#pragma once
#include "STILOQ/Structs/SolverStructs/SolverAnalysisInput.h"

namespace STILOQ
{
    struct Job
    {
        SolverType solverType;
        SolverAnalysisInput input;
    };
}