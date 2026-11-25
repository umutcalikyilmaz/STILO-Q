#pragma once
#include <set>
#include "STILO-Q/Structs/AnalyzerStructs/DistributionConfiguration.h"

namespace QEVO
{
    struct TxnSPConfigurationSet
    {
        std::set<int> jobCounts;
        std::set<int> machineCounts;
        std::set<double> conflictPopensities;
        std::set<DistributionConfiguration> jobLengthDistributions;
    };
    
}