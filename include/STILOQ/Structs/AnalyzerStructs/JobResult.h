#pragma once
#include <chrono>
#include <map>
#include "STILOQ/Structs/SolverStructs/SolverAnalysisInput.h"

namespace STILOQ
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