#pragma once
#include "STILOQ/ProblemModels/Problem.h"
#include "STILOQ/Shared/Parameters.h"
#include "STILOQ/Shared/EvolvingParameter.h"
#include "STILOQ/SolverModels/SAModels/DistanceCalculator.h"

namespace STILOQ
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