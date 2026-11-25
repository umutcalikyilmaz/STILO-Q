#pragma once
#include <chrono>
#include <set>
#include "STILOQ/Structs/AnalyzerStructs/ACOConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/GAConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/SAConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/QEAConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/SolverSelection.h"

namespace STILOQ
{
    struct InstanceAnalysisInput
    {
        std::set<std::chrono::milliseconds> timeLimits;
        SolverSelection solverSelection;
        ACOConfigurationSet ACOConfigurations;
        GAConfigurationSet GAConfigurations;
        SAConfigurationSet SAConfigurations;
        QEAConfigurationSet BQEAConfigurations;
        QEAConfigurationSet MQEAConfigurations;
        std::string instanceDirectory;
        int solutionCount;
        int threadCount;
        ProblemType problemType;
    };
}