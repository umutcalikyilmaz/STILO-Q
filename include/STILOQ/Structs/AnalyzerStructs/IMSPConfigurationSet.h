#pragma once
#include <set>
#include "STILOQ/Structs/AnalyzerStructs/DistributionConfiguration.h"

namespace STILOQ
{
    struct IMSPConfigurationSet
    {
        std::set<int> jobCounts;
        std::set<int> machineCounts;
        std::set<DistributionConfiguration> jobLengthDistributions;
    };
    
}