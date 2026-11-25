#pragma once
#include <set>
#include "STILOQ/Structs/AnalyzerStructs/DistributionConfiguration.h"

namespace STILOQ
{
    struct TxnSPConfigurationSet
    {
        std::set<int> jobCounts;
        std::set<int> machineCounts;
        std::set<double> conflictPopensities;
        std::set<DistributionConfiguration> jobLengthDistributions;
    };
    
}