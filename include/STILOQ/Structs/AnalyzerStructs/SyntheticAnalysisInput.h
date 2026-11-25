#pragma once
#include <chrono>
#include <set>
#include "STILOQ/Structs/AnalyzerStructs/IMSPConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/MCPConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/TSPConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/TxnSPConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/WMCPConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/ACOConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/GAConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/SAConfigurationSet.h"
#include "STILOQ/Structs/AnalyzerStructs/ProblemSelection.h"
#include "STILOQ/Structs/AnalyzerStructs/SolverSelection.h"

namespace STILOQ
{
    struct SyntheticAnalysisInput
    {
        std::set<std::chrono::milliseconds> timeLimits;
        ProblemSelection problemSelection;
        IMSPConfigurationSet IMSPconfigurations;
        MCPConfigurationSet MCPConfigurations;
        TSPConfigurationSet TSPConfigurations;
        TxnSPConfigurationSet TxnSPConfigurations;
        WMCPConfigurationSet WMCPConfigurations;
        SolverSelection solverSelection;
        ACOConfigurationSet ACOConfigurations;
        GAConfigurationSet GAConfigurations;
        SAConfigurationSet SAConfigurations;
        QEAConfigurationSet BQEAConfigurations;
        QEAConfigurationSet MQEAConfigurations; 
        int problemCount;
        int threadCount;
        bool useExisting;
    };
}