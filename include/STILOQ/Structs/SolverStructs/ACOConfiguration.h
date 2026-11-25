#pragma once
#include "STILOQ/Shared/Parameters.h"

namespace STILOQ
{
    struct ACOConfiguration
    {
        double pheromoneInfluence;
        double heuristicInfluence;
        double evaporationParameter;
        double localDecayParameter;
        double pheromoneConstant; 
        double initialPheromone;
        int antCount;
        int eliteCount;
        GraphStructure graphStructure; 
        PheromoneUpdateOperator pheromoneUpdateOperator;
        PheromoneCalculationOperator pheromoneCalculationOperator;
        TourSelectionOperator tourSelectionOperator;
    };    
}