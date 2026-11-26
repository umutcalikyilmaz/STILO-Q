#pragma once
#include <set>
#include "STILOQ/Structs/SolverStructs/QEAConfiguration.h"

namespace STILOQ
{
    struct QEAConfigurationSet
    {
        std::set<double> baseGlobalBestAngles;
        std::set<double> baseIterationBestAngles;
        std::set<double> baseModelBestAngles;
        std::set<double> limitAngles;
        std::set<double> heuristicInfluences;
        std::set<int> measurementCounts;
        std::set<int> populationSizes;
        std::set<int> globalBestRotationCounts;
        std::set<int> iterationBestRotationCounts;
        std::set<int> modelBestRotationCounts;
        std::set<AngleCalculationOperator> angleCalculationOperators;
        std::set<GraphStructure> graphStructures;
        std::set<InitializationType> initializationTypes;
    };
    
}