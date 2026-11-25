#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include "STILO-Q/ProblemModels/IdenticalMachinesSchedulingProblem.h"
#include "STILO-Q/ProblemModels/MaxCutProblem.h"
#include "STILO-Q/ProblemModels/TransactionSchedulingProblem.h"
#include "STILO-Q/ProblemModels/TravelingSalespersonProblem.h"
#include "STILO-Q/ProblemModels/WeightedMaxCutProblem.h"
#include "STILO-Q/Shared/Exceptions.h"
#include "STILO-Q/Shared/Parameters.h"

namespace fs = std::filesystem;

namespace QEVO
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