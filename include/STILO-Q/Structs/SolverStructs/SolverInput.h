#pragma once
#include <memory>
#include "STILO-Q/Structs/SolverStructs/ACOConfiguration.h"
#include "STILO-Q/Structs/SolverStructs/GAConfiguration.h"
#include "STILO-Q/Structs/SolverStructs/QEAConfiguration.h"
#include "STILO-Q/Structs/SolverStructs/SAConfiguration.h"
#include "STILO-Q/ProblemModels/Problem.h"

namespace QEVO
{
    struct SolverInput
    {
        Problem* problem;
        ACOConfiguration ACOConfig;
        GAConfiguration GAConfig;
        SAConfiguration SAConfig;
        QEAConfiguration QEAConfig;
        std::chrono::milliseconds timeLimit;
    };    
}