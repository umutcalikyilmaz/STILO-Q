#pragma once
#include "STILOQ/Shared/Parameters.h"

namespace STILOQ
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
        InitializationType initializationType;
    };
    
}