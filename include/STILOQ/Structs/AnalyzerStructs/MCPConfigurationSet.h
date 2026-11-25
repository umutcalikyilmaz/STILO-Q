#pragma once
#include <set>

namespace STILOQ
{
    struct MCPConfigurationSet
    {
        std::set<int> vertexCounts;
        std::set<double> edgePropensities;
    };
    
}