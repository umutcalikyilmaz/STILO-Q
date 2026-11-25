#pragma once
#include <queue>
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <sstream>
#include <syncstream>
#include <queue>
#include <thread>
#include <tuple>
#include "STILOQ/Analyzers/JobCreator.h"
#include "STILOQ/ProblemModels/IdenticalMachinesSchedulingProblem.h"
#include "STILOQ/ProblemModels/MaxCutProblem.h"
#include "STILOQ/ProblemModels/TransactionSchedulingProblem.h"
#include "STILOQ/ProblemModels/TravelingSalespersonProblem.h"
#include "STILOQ/ProblemModels/WeightedMaxCutProblem.h"
#include "STILOQ/Solvers/ACOSolver.h"
#include "STILOQ/Solvers/GASolver.h"
#include "STILOQ/Solvers/SASolver.h"
#include "STILOQ/Solvers/BQEASolver.h"
#include "STILOQ/Solvers/MQEASolver.h"
#include "STILOQ/Structs/AnalyzerStructs/JobResult.h"
#include "STILOQ/Structs/AnalyzerStructs/SyntheticAnalysisInput.h"

namespace STILOQ
{
    class Analyzer
    {
    protected:
        
        std::queue<std::unique_ptr<Job>> jobQueue;
        SolverSelection solverSelection;
        ACOConfigurationSet acoConfigurations;
        GAConfigurationSet gaConfigurations;
        SAConfigurationSet saConfigurations;
        QEAConfigurationSet bqeaConfigurations;
        QEAConfigurationSet mqeaConfigurations;
        std::set<std::chrono::milliseconds> timeLimits;
        int threadCount;
        int runCount;        
        bool minimization;

        void createJobs();

        std::unique_ptr<Solver> createSolver(SolverType solverType);

        bool pullJob(std::unique_ptr<Job>& job);

        void pushResult(JobResult result);

        void initializeJobResult(JobResult& result);

        void initializeJob(Job* job, JobResult& result);

        void updateJobResult(const SolverAnalysisOutput& analysisOutput, JobResult& result, bool minimum);

        void finalizeJobResult(JobResult& result, int count);

        void displayResult(int& batchCount);

        void displayFinal(int batchCount);

        bool compareObjectiveValues(double currentValue, double newValue);

        void runAnalyzerThread();

        void initializeRun();

        std::string getACOString(const ACOConfiguration& acoConfig);

        std::string getGAString(const GAConfiguration& gaConfig);

        std::string getSAString(const SAConfiguration& saConfig);

        std::string getQEAString(const QEAConfiguration& qeaConfig);

        void printAnalysisResults(const std::string& analysisType, const std::string& problemClass,
            const std::string& instanceClass);

    private:
        
        std::mutex jobQueueLock_;
        std::mutex resultQueueLock_;
        std::mutex displayLock_;
        std::condition_variable resultQueueCondition_;        
        JobCreator jobCreator_;        
        std::queue<JobResult> resultQueue_;
        std::queue<JobResult> writeQueue_;        
        std::map<std::chrono::milliseconds, std::tuple<double, double, double, int, ACOConfiguration>> bestACOConfigurations_;
        std::map<std::chrono::milliseconds, std::tuple<double, double, double, int, GAConfiguration>> bestGAConfigurations_;
        std::map<std::chrono::milliseconds, std::tuple<double, double, double, int, SAConfiguration>> bestSAConfigurations_;
        std::map<std::chrono::milliseconds, std::tuple<double, double, double, int, QEAConfiguration>> bestBQEAConfigurations_;
        std::map<std::chrono::milliseconds, std::tuple<double, double, double, int, QEAConfiguration>> bestMQEAConfigurations_;
        int jobCount_;
        int completed_;
        int completedThreadCount_;
    };
}