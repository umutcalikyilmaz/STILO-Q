#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include "STILO-Q/ProblemModels/IdenticalMachinesSchedulingProblem.h"
#include "STILO-Q/ProblemModels/MaxCutProblem.h"
#include "STILO-Q/ProblemModels/TransactionSchedulingProblem.h"
#include "STILO-Q/ProblemModels/TravelingSalespersonProblem.h"
#include "STILO-Q/ProblemModels/WeightedMaxCutProblem.h"
#include "STILO-Q/Shared/Parameters.h"
#include "STILO-Q/Shared/Parse.h"

namespace fs = std::filesystem;

namespace QEVO
{
    class ProblemCreator
    {
    public:
    
        std::vector<std::unique_ptr<Problem>> createIMSP(int problemCount, bool useExisting, int jobCount,
            int machineCount, ProbabilityDistribution lengthDistribution, double distributionParanmeter1,
            double distributionParameter2);

        std::vector<std::unique_ptr<Problem>> createMCP(int problemCount, bool useExisting, int verticeCount,
            double edgePropensity);

        std::vector<std::unique_ptr<Problem>> createTxnSP(int problemCount, bool useExisting, int jobCount,
            int machineCount, double conflictPropensity, ProbabilityDistribution lengthDistribution,
            double distributionParameter1, double distributionParameter2);

        std::vector<std::unique_ptr<Problem>> createTSP(int problemCount, bool useExisting, int cityCount,
            ProbabilityDistribution distanceDistribution, double distributionParameter1,
            double distributionParameter2);

        std::vector<std::unique_ptr<Problem>> createWMCP(int problemCount, bool useExisting, int verticeCount,
            ProbabilityDistribution weightDistribution, double distributionParameter1,
            double distributionParameter2);

    private:

        fstream file_;

        void openFile(string problem, string fileName, bool in);

        std::unique_ptr<Problem> pullIMSP(int jobCount, int machineCount, const string& line);

        std::unique_ptr<Problem> pushIMSP(int jobCount, int machineCount,
            ProbabilityDistribution lengthDistribution, double distributionParanmeter1,
            double distributionParameter2);

        std::unique_ptr<Problem> pullMCP(int verticeCount, const string& line);

        std::unique_ptr<Problem> pushMCP(int verticeCount, double edgePropensity);

        std::unique_ptr<Problem> pullTxnSP(int jobCount, int machineCount, const string& line);

        std::unique_ptr<Problem> pushTxnSP(int jobCount, int machineCount, double conflictPropensity,
            ProbabilityDistribution lengthDistribution, double distributionParameter1,
            double distributionParameter2);

        std::unique_ptr<Problem> pullTSP(int cityCount, const string& line);

        std::unique_ptr<Problem> pushTSP(int cityCount, ProbabilityDistribution distanceDistribution,
            double distributionParameter1, double distributionParameter2);

        std::unique_ptr<Problem> pullWMCP(int verticeCount, const string& line);

        std::unique_ptr<Problem> pushWMCP(int verticeCount, ProbabilityDistribution weightDistribution,
            double distributionParameter1, double distributionParameter2);
    };
}