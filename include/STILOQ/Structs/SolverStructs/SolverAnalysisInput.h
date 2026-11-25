#pragma once
#include <memory>
#include <set>
#include "STILOQ/Structs/SolverStructs/ACOConfiguration.h"
#include "STILOQ/Structs/SolverStructs/GAConfiguration.h"
#include "STILOQ/Structs/SolverStructs/QEAConfiguration.h"
#include "STILOQ/Structs/SolverStructs/SAConfiguration.h"
#include "STILOQ/ProblemModels/Problem.h"

namespace STILOQ
{
    struct SolverAnalysisInput
    {
        Problem* problem;
        ACOConfiguration ACOConfig;
        GAConfiguration GAConfig;
        SAConfiguration SAConfig;
        QEAConfiguration QEAConfig;
        std::set<std::chrono::milliseconds> timeLimits;
    };    
}