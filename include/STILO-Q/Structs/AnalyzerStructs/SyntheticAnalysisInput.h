#pragma once
#include <chrono>
#include <set>
#include "STILO-Q/Structs/AnalyzerStructs/IMSPConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/MCPConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/TSPConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/TxnSPConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/WMCPConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/ACOConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/GAConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/SAConfigurationSet.h"
#include "STILO-Q/Structs/AnalyzerStructs/ProblemSelection.h"
#include "STILO-Q/Structs/AnalyzerStructs/SolverSelection.h"

namespace QEVO
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