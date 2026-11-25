#pragma once
#include "STILO-Q/Shared/Parameters.h"

namespace QEVO
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