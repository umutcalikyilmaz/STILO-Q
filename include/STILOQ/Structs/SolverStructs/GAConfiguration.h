#pragma once
#include "STILOQ/Shared/Parameters.h"

namespace STILOQ
{
    struct GAConfiguration
    {
        double mutationProbability;
        double pointMutationProbability;
        double pointMutationCoefficient;
        double insertMutationCoefficient;
        double invertMutationCoefficient;
        double swapMutationCoefficient;
        double cycleCrossoverCoefficient;
        double kpointCrossoverCoefficient;
        double OXCrossoverCoefficient;
        double PMXCrossoverCoefficient;
        double uniformCrossoverCoefficient;
        int populationSize;
        int matingCount;
        int eliteCount; 
        int k;
        int tournamentSize;
        SelectionOperator selectionOperator;
    };
    
}