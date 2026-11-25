#pragma once
#include "STILO-Q/ProblemModels/Problem.h"
#include "STILO-Q/Shared/Parameters.h"
#include "STILO-Q/Shared/EvolvingParameter.h"
#include "STILO-Q/SolverModels/SAModels/DistanceCalculator.h"

namespace QEVO
{
    class SAIteratorWrapper
    {
    public:

        class SASolverKey
        {
        private:

            SASolverKey() { }

            friend class SASolver;
        };
    
        virtual void iterate(SASolverKey) = 0;

        virtual SolverOutput getOutput(SASolverKey) = 0;

        virtual ~SAIteratorWrapper() = default;
    };
}