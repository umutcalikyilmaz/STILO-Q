#pragma once
#include <memory>
#include "STILOQ/Structs/SolverStructs/ACOConfiguration.h"
#include "STILOQ/Structs/SolverStructs/GAConfiguration.h"
#include "STILOQ/Structs/SolverStructs/QEAConfiguration.h"
#include "STILOQ/Structs/SolverStructs/SAConfiguration.h"
#include "STILOQ/ProblemModels/Problem.h"

namespace STILOQ
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