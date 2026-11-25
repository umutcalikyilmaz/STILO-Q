#pragma once
#include "STILOQ/Analyzers/Analyzer.h"
#include "STILOQ/Analyzers/InstanceReader.h"
#include "STILOQ/Structs/AnalyzerStructs/InstanceAnalysisInput.h"

namespace STILOQ
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

