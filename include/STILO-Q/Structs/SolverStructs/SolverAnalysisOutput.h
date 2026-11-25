#pragma once
#include <map>
#include "STILO-Q/Structs/SolverStructs/SolverOutput.h"

namespace QEVO
{
    struct SolverAnalysisOutput
    {
        std::map<std::chrono::milliseconds, std::unique_ptr<SolverOutput>> results;
    };    
}