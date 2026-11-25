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
#include "STILO-Q/Analyzers/JobCreator.h"
#include "STILO-Q/ProblemModels/IdenticalMachinesSchedulingProblem.h"
#include "STILO-Q/ProblemModels/MaxCutProblem.h"
#include "STILO-Q/ProblemModels/TransactionSchedulingProblem.h"
#include "STILO-Q/ProblemModels/TravelingSalespersonProblem.h"
#include "STILO-Q/ProblemModels/WeightedMaxCutProblem.h"
#include "STILO-Q/Solvers/ACOSolver.h"
#include "STILO-Q/Solvers/GASolver.h"
#include "STILO-Q/Solvers/SASolver.h"
#include "STILO-Q/Solvers/BQEASolver.h"
#include "STILO-Q/Solvers/MQEASolver.h"
#include "STILO-Q/Structs/AnalyzerStructs/JobResult.h"
#include "STILO-Q/Structs/AnalyzerStructs/SyntheticAnalysisInput.h"

namespace QEVO
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