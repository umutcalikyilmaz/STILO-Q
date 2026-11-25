#pragma once
#include <memory>
#include <set>
#include "STILO-Q/Structs/SolverStructs/ACOConfiguration.h"
#include "STILO-Q/Structs/SolverStructs/GAConfiguration.h"
#include "STILO-Q/Structs/SolverStructs/QEAConfiguration.h"
#include "STILO-Q/Structs/SolverStructs/SAConfiguration.h"
#include "STILO-Q/ProblemModels/Problem.h"

namespace QEVO
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