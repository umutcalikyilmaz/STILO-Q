#pragma once
#include "STILO-Q/Structs/SolverStructs/SolverOutput.h"

namespace QEVO
{
    class PopulationWrapper
    {
    public:

        class GASolverKey
        {
        private:

            GASolverKey() { }

            friend class GASolver;
        };

        virtual void evolve(GASolverKey) = 0;

        virtual SolverOutput getOutput(GASolverKey) = 0;

        virtual ~PopulationWrapper() = default;
    };
}
