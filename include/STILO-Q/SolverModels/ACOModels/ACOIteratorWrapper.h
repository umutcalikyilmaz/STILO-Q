#pragma once
#include "STILO-Q/SolverModels/ACOModels/Pheromone.h"
#include "STILO-Q/ProblemModels/Problem.h"
#include "STILO-Q/Structs/SolverStructs/SolverOutput.h"

namespace QEVO
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