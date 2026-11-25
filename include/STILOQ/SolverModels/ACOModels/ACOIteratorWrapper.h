#pragma once
#include "STILOQ/SolverModels/ACOModels/Pheromone.h"
#include "STILOQ/ProblemModels/Problem.h"
#include "STILOQ/Structs/SolverStructs/SolverOutput.h"

namespace STILOQ
{
    class ACOIteratorWrapper
    {
    public:

        class ACOSolverKey
        {
        private:
            
            ACOSolverKey() { }

            friend class ACOSolver;
        };

        virtual void iterate(ACOSolverKey) = 0;
        
        virtual SolverOutput getOutput(ACOSolverKey) = 0;

        virtual ~ACOIteratorWrapper() = default;
    };
}