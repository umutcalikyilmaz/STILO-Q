#pragma once
#include <set>
#include "STILO-Q/Structs/AnalyzerStructs/DistributionConfiguration.h"

namespace QEVO
{
    struct WMCPConfigurationSet
    {
        std::set<int> vertexCounts;
        std::set<DistributionConfiguration> weightDistributions;
    };
    
}