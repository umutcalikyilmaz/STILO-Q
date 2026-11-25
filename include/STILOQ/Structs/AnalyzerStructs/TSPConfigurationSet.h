#pragma once
#include <set>
#include "STILOQ/Structs/AnalyzerStructs/DistributionConfiguration.h"

namespace STILOQ
{
    struct TSPConfigurationSet
    {
        std::set<int> cityCounts;
        std::set<DistributionConfiguration> distanceDistributions;
    };
    
}