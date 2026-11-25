#pragma once
#include <map>
#include "STILOQ/Structs/SolverStructs/SolverOutput.h"

namespace STILOQ
{
    struct SolverAnalysisOutput
    {
        std::map<std::chrono::milliseconds, std::unique_ptr<SolverOutput>> results;
    };    
}