#pragma once
#include "STILO-Q/Shared/Parameters.h"

namespace QEVO
{
    struct QEAConfiguration
    {
        double baseGlobalBestAngle;
        double baseIterationBestAngle;
        double baseModelBestAngle;
        double limitAngle;
        double heuristicInfluence;
        int measurementCount;
        int populationSize;
        int globalBestRotationCount;
        int iterationBestRotationCount;
        int modelBestRotationCount;
        AngleCalculationOperator angleCalculationOperator;
        GraphStructure graphStructure;
        InitializationOperator initializationType;
    };
    
}