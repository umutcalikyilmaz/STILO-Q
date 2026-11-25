#pragma once
#include <set>
#include "STILO-Q/Structs/AnalyzerStructs/DistributionConfiguration.h"

namespace QEVO
{
    struct IMSPConfigurationSet
    {
        std::set<int> jobCounts;
        std::set<int> machineCounts;
        std::set<DistributionConfiguration> jobLengthDistributions;
    };
    
}