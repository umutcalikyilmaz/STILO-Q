#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include "STILOQ/ProblemModels/IdenticalMachinesSchedulingProblem.h"
#include "STILOQ/ProblemModels/MaxCutProblem.h"
#include "STILOQ/ProblemModels/TransactionSchedulingProblem.h"
#include "STILOQ/ProblemModels/TravelingSalespersonProblem.h"
#include "STILOQ/ProblemModels/WeightedMaxCutProblem.h"
#include "STILOQ/Shared/Exceptions.h"
#include "STILOQ/Shared/Parameters.h"

namespace fs = std::filesystem;

namespace STILOQ
{
    class InstanceReader
    {
    public:
    
        std::vector<std::tuple<std::unique_ptr<Problem>, std::string, double>> readInstances(ProblemType pt, std::string instanceDirectory);

        ProblemType getProblemType();

    private:

        ProblemType lastProbemType_;
        tuple<std::unique_ptr<Problem>, std::string, double> (InstanceReader::*readFilePtr_)(fstream&, std::string&);

        tuple<std::unique_ptr<Problem>, std::string, double> readIMSP(fstream& file, std::string& instanceName);

        tuple<std::unique_ptr<Problem>, std::string, double> readMCP(fstream& file, std::string& instanceName);

        tuple<std::unique_ptr<Problem>, std::string, double> readTxnSP(fstream& file, std::string& instanceName);

        tuple<std::unique_ptr<Problem>, std::string, double> readTSP(fstream& file, std::string& instanceName);

        tuple<std::unique_ptr<Problem>, std::string, double> readWMCP(fstream& file, std::string& instanceName);
    };
}