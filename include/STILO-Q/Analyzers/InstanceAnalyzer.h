#pragma once
#include "STILO-Q/Analyzers/Analyzer.h"
#include "STILO-Q/Analyzers/InstanceReader.h"
#include "STILO-Q/Structs/AnalyzerStructs/InstanceAnalysisInput.h"

namespace QEVO
{
    class InstanceAnalyzer final : public Analyzer
    {
    public:

        void analyze(const InstanceAnalysisInput& input);

    private:

        InstanceReader reader_;
        std::vector<std::unique_ptr<Problem>> problems_;
        std::string instanceDirectory_;
        ProblemType problemType_;
        
        void createProblems(std::unique_ptr<Problem> problem);

        void runSolverThread(Problem* problem, int solutionCount, bool min);

        void runThreads();

        void printOptimalValue(double);

        void runProblems();
    };
}

