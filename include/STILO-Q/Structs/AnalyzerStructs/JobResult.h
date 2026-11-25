#pragma once
#include <chrono>
#include <map>
#include "STILO-Q/Structs/SolverStructs/SolverAnalysisInput.h"

namespace QEVO
{
    struct JobOutput
    {
        double averageTime;
        double averageCost;
        double averageIterationCount;
        double successRate;
    };
    
    struct JobResult
    {
        SolverType solverType;
        SolverAnalysisInput input;
        std::map<std::chrono::milliseconds, JobOutput> results;
    };   
}