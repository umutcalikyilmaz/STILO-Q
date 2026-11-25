#pragma once
#include <set>

namespace QEVO
{
    struct MCPConfigurationSet
    {
        std::set<int> vertexCounts;
        std::set<double> edgePropensities;
    };
    
}