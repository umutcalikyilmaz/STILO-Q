#pragma once
#include "STILO-Q/Shared/Parameters.h"

namespace QEVO
{
    struct DistributionConfiguration
    {
        ProbabilityDistribution distributionType;
        double distributionParameter1;
        double distributionParameter2;

        DistributionConfiguration(ProbabilityDistribution distributionType, double distributionParameter1,
            double distributionParameter2)
            : distributionType(distributionType),
              distributionParameter1(distributionParameter1),
              distributionParameter2(distributionParameter2) { }

        auto operator<=>(const DistributionConfiguration&) const = default;
    };
    
}