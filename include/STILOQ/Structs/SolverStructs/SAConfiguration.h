#pragma once
#include "STILOQ/Shared/Parameters.h"

namespace STILOQ
{
    struct SAConfiguration
    {
        double pointMoveCoefficient;
        double insertMoveCoefficient;
        double invertMoveCoefficient;
        double swapMoveCoefficient;
        double initialTemperature;
        double coolingParameter;
        int minimumNeighborhoodSize;
        int maximumNeighborhoodSize;
        EvolvingParameterType coolingSchedule;
        DistanceCalculationOperator distanceCalculationOperator;
    };
}