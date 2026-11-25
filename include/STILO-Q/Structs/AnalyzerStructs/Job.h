#pragma once
#include "STILO-Q/Structs/SolverStructs/SolverAnalysisInput.h"

namespace QEVO
{
    struct Job
    {
        SolverType solverType;
        SolverAnalysisInput input;
    };
}