#pragma once
#include "STILOQ/ProblemModels/Problem.h"

namespace STILOQ
{
    class MaxCutProblem final : public Problem
    {
    public:

        MaxCutProblem(const MaxCutProblem& original);

        MaxCutProblem(int verticeCount, const std::vector<std::vector<uint8_t>>& edges);

        MaxCutProblem(int verticeCount, double edgePropensity);

        double getCost(std::span<const int> solution) override;

        double getValue(double cost) override;

        double getScaleFactor() override;

        friend class ProblemCreator;

    private:

        std::vector<std::vector<uint8_t>> edges_;
        double maxEdge_;

        void getHeuristicValuesMultiStage(std::vector<std::vector<std::vector<double>>>& heuristic);

        void getHeuristicValuesSingleStage(std::vector<std::vector<std::vector<double>>>& heuristic);

        void getHeuristicValuesSequential(std::vector<std::vector<std::vector<double>>>& heuristic);
    };
}