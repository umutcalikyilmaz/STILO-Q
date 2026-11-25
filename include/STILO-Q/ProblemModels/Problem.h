#pragma once
#include <cstdint>
#include <boost/multiprecision/cpp_int.hpp>
#include "STILO-Q/SolverModels/GraphModels/Graph.h"
#include "STILO-Q/Shared/Random.h"
#include "STILO-Q/Shared/Containers.h"

namespace QEVO
{
    class Problem
    {
    public:

        Problem(const Problem& original);

        Problem(int stringLength, int stateCount, SolutionSpaceType solutionSpaceType);

        virtual double getCost(std::span<const int> solution) = 0;

        virtual double getValue(double cost) = 0;

        int getStringLength();

        int getStateCount();

        SolutionSpaceType getSolutionSpaceType();

        void getHeuristicValues(GraphStructure gt, std::vector<std::vector<std::vector<double>>>& heuristic);

        virtual double getScaleFactor() = 0;

        virtual ~Problem() = default;

    protected:

        std::vector<int> pstate;
        std::vector<int> slist;
        int stringLength;
        int stateCount;
        SolutionSpaceType solutionSpaceType;

        virtual void getHeuristicValuesMultiStage(std::vector<std::vector<std::vector<double>>>& heuristic) = 0;

        virtual void getHeuristicValuesSingleStage(std::vector<std::vector<std::vector<double>>>& heuristic) = 0;

        virtual void getHeuristicValuesSequential(std::vector<std::vector<std::vector<double>>>& heuristic) = 0;
    };
}