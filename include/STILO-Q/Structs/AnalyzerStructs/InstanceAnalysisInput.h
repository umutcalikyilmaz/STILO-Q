#pragma once
#include <chrono>
#include <set>
#include "STILO-Q/Structs/AnalyzerStructs/ACOConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/GAConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/SAConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/QEAConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/SolverSelection.h"

namespace QEVO
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