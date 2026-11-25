#pragma once
#include <set>
#include "STILOQ/Structs/AnalyzerStructs/DistributionConfiguration.h"

namespace STILOQ
{
    struct WMCPConfigurationSet
    {
        std::set<int> vertexCounts;
        std::set<DistributionConfiguration> weightDistributions;
    };
    
}