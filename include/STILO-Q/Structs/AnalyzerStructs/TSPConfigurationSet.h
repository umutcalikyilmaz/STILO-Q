#pragma once
#include <set>
#include "STILO-Q/Structs/AnalyzerStructs/DistributionConfiguration.h"

namespace QEVO
{
    struct TSPConfigurationSet
    {
        std::set<int> cityCounts;
        std::set<DistributionConfiguration> distanceDistributions;
    };
    
}