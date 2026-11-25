#include "STILO-Q/Analyzers/SyntheticAnalyzer.h"

namespace QEVO
{
    void SyntheticAnalyzer::createIMSP(int jobCount, int machineCount, ProbabilityDistribution lengthDistribution,
        double distributionParameter1, double distributionParameter2)
    {
        problems_.push_back(problemCreator_.createIMSP(runCount, useExisting_, jobCount, machineCount,
            lengthDistribution, distributionParameter1, distributionParameter2));

        for(int i = 1; i < threadCount; i++)
        {
            problems_.push_back(std::vector<std::unique_ptr<Problem>>());

            for(int j = 0; j < runCount; j++)
            {
                problems_[i].push_back(std::make_unique<IdenticalMachinesSchedulingProblem>((*static_cast<IdenticalMachinesSchedulingProblem*>(problems_[0][j].get()))));
            }
        }
    }

    void SyntheticAnalyzer::createMCP(int verticeCount, double edgePropensity)
    {
        problems_.push_back(problemCreator_.createMCP(runCount, useExisting_, verticeCount,
            edgePropensity));

        for(int i = 1; i < threadCount; i++)
        {
            problems_.push_back(std::vector<std::unique_ptr<Problem>>());
    
            for(int j = 0; j < runCount; j++)
            {
                problems_[i].push_back(std::make_unique<MaxCutProblem>((*static_cast<MaxCutProblem*>(problems_[0][j].get()))));
            }
        }
    }

    void SyntheticAnalyzer::createTxnSP(int jobCount, int machineCount, double conflictPropensity,
        ProbabilityDistribution lengthDistribution, double distributionParameter1, double distributionParameter2)
    {
        problems_.push_back(problemCreator_.createTxnSP(runCount, useExisting_, jobCount, machineCount,
            conflictPropensity, lengthDistribution, distributionParameter1, distributionParameter2));

        for(int i = 1; i < threadCount; i++)
        {
            problems_.push_back(std::vector<std::unique_ptr<Problem>>());
        
            for(int j = 0; j < runCount; j++)
            {
                problems_[i].push_back(std::make_unique<TransactionSchedulingProblem>((*static_cast<TransactionSchedulingProblem*>(problems_[0][j].get()))));
            }
        }
    }

    void SyntheticAnalyzer::createTSP(int cityCount, ProbabilityDistribution distanceDistribution,
        double distributionParameter1, double distributionParameter2)
    {
        problems_.push_back(problemCreator_.createTSP(runCount, useExisting_, cityCount,
            distanceDistribution, distributionParameter1, distributionParameter2));

        for(int i = 1; i < threadCount; i++)
        {
            problems_.push_back(std::vector<std::unique_ptr<Problem>>());
        
            for(int j = 0; j < runCount; j++)
            {
                problems_[i].push_back(std::make_unique<TravelingSalespersonProblem>((*static_cast<TravelingSalespersonProblem*>(problems_[0][j].get()))));
            }
        }
    }

    void SyntheticAnalyzer::createWMCP(int verticeCount, ProbabilityDistribution weightDistribution,
        double distributionParameter1, double distributionParameter2)
    {
        problems_.push_back(problemCreator_.createWMCP(runCount, useExisting_, verticeCount,
            weightDistribution, distributionParameter1, distributionParameter2));

        for(int i = 1; i < threadCount; i++)
        {
            problems_.push_back(std::vector<std::unique_ptr<Problem>>());
        
            for(int j = 0; j < runCount; j++)
            {
                problems_[i].push_back(std::make_unique<WeightedMaxCutProblem>((*static_cast<WeightedMaxCutProblem*>(problems_[0][j].get()))));
            }
        }
    }

    void SyntheticAnalyzer::runSolverThread(const std::vector<std::unique_ptr<Problem>>& problems,
        int problemCount, bool min)
    {
        std::unique_ptr<Solver> solver = createSolver(SolverType::AntColonyOptimization);
        std::unique_ptr<Job> job;
        SolverType solverType = SolverType::AntColonyOptimization;
        int batchCount = 0;

        while(pullJob(job))
        {
            if(job->solverType != solverType)
            {
                solverType = job->solverType;
                solver = createSolver(solverType);
            }

            JobResult jobResult;
            initializeJob(job.get(), jobResult);

            try
            {
                for(int i = 0; i < problemCount; i++)
                {
                    job->input.problem = problems[i].get();
                    auto output = solver->analyze(job->input);
                    updateJobResult(output, jobResult, min);
                }

                finalizeJobResult(jobResult, problemCount);
                pushResult(move(jobResult));
            }
            catch(const InvalidParameterException& e) { }
            
            displayResult(++batchCount);
        }

        displayFinal(batchCount);
    }

    void SyntheticAnalyzer::runThreads()
    {
        initializeRun();
        std::vector<std::jthread> threads;
        threads.reserve(threadCount);      

        for(int i = 0; i < threadCount; i++)
        {
            threads.emplace_back(jthread([this, &problem = problems_[i]]()
            {
                this->runSolverThread(problem, runCount, minimization);
            }));
        }

        runAnalyzerThread();
        threads.clear();
    }

    void SyntheticAnalyzer::runIMSP(const IMSPConfigurationSet& params)
    {
        minimization = true;

        for(int jn : params.jobCounts)
        {
            for(int mn : params.machineCounts)
            {
                for(auto dist : params.jobLengthDistributions)
                {
                    sstream_ << "IMSP_" << jn << "_" << mn << "_" <<
                        (dist.distributionType == ProbabilityDistribution::Normal ? "n" : "u") << "_" <<
                        dist.distributionParameter1 << "_" << dist.distributionParameter2;
                    std::cout << "\n\n----Working on " + sstream_.str() + "----\n";                    
                    createIMSP(jn, mn, dist.distributionType, dist.distributionParameter1, dist.distributionParameter2);
                    createJobs();
                    runThreads();
                    printAnalysisResults("synthetic", "IMSP", sstream_.str());
                    sstream_.str(string());
                    problems_.clear();
                }
            }
        }
    }

    void SyntheticAnalyzer::runMCP(const MCPConfigurationSet& params)
    {
        minimization = false;

        for(int vn : params.vertexCounts)
        {
            for(double ep : params.edgePropensities)
            {
                sstream_ << "MCP_" << vn << "_" << ep;
                std::cout << "\n\n----Working on " + sstream_.str() + "----\n"; 
                createMCP(vn, ep);
                createJobs();
                runThreads();
                printAnalysisResults("synthetic", "MCP", sstream_.str());
                sstream_.str(string());
                problems_.clear();
            }
        }
    }

    void SyntheticAnalyzer::runTxnSP(const TxnSPConfigurationSet& params)
    {
        minimization = true;

        for(int jn : params.jobCounts)
        {
            for(int mn : params.machineCounts)
            {
                for(double cp : params.conflictPopensities)
                {
                    for(auto& dist : params.jobLengthDistributions)
                    {
                        sstream_ << "TxnSP_" << jn << "_" << mn << "_" << cp <<
                            (dist.distributionType == ProbabilityDistribution::Normal ? "_n_" : "_u_") <<
                            dist.distributionParameter1 << "_" << dist.distributionParameter2;
                        cout << "\n\n----Working on " + sstream_.str() + "----\n";
                        createTxnSP(jn, mn, cp, dist.distributionType, dist.distributionParameter1, dist.distributionParameter2);
                        createJobs();
                        runThreads();
                        printAnalysisResults("synthetic", "TxnSP", sstream_.str());
                        sstream_.str(string());
                        problems_.clear();
                    }
                }
            }
        }
    }

    void SyntheticAnalyzer::runTSP(const TSPConfigurationSet& params)
    {
        minimization = true;

        for(int cn : params.cityCounts)
        {
            for(auto& dist : params.distanceDistributions)
            {
                sstream_ << "TSP_" << cn << "_" << (dist.distributionType == ProbabilityDistribution::Normal ? "_n_" : "_u_")
                    << "_" << dist.distributionParameter1 << "_" << dist.distributionParameter2;
                cout << "\n\n----Working on " + sstream_.str() + "----\n";
                createTSP(cn, dist.distributionType, dist.distributionParameter1, dist.distributionParameter2);
                createJobs();
                runThreads();
                printAnalysisResults("synthetic", "TSP", sstream_.str());
                sstream_.str(string());
                problems_.clear();
            }
        }
    }

    void SyntheticAnalyzer::runWMCP(const WMCPConfigurationSet& params)
    {
        minimization = false;

        for(int vn : params.vertexCounts)
        {
            for(auto& dist : params.weightDistributions)
            {
                sstream_ << "WMCP_" << vn << "_" <<
                    (dist.distributionType == ProbabilityDistribution::Normal ? "_n_" : "_u_")
                    << "_" << dist.distributionParameter1 << "_" << dist.distributionParameter2;
                std::cout << "\n\n----Working on " + sstream_.str() + "----\n";
                createWMCP(vn, dist.distributionType, dist.distributionParameter1, dist.distributionParameter2);
                createJobs();
                runThreads();
                printAnalysisResults("synthetic", "WMCP", sstream_.str());
                sstream_.str(string());
                problems_.clear();
            }
        }
    }

    void SyntheticAnalyzer::runProblems()
    {
        if(problemSelection_.analyzeIMSP)
        {
            runIMSP(imspConfigurations_);
        }

        if(problemSelection_.analyzeMCP)
        {
            runMCP(mcpConfigurations_);
        }

        if(problemSelection_.analyzeTxnSP)
        {
            runTxnSP(txnspConfigurations_);
        }

        if(problemSelection_.analyzeTSP)
        {
            runTSP(tspConfigurations_);
        }

        if(problemSelection_.analyzeWMCP)
        {
            runWMCP(wmcpConfigurations_);
        }
    }

    void SyntheticAnalyzer::analyze(const SyntheticAnalysisInput& input)
    {
        solverSelection = input.solverSelection;
        acoConfigurations = input.ACOConfigurations;
        gaConfigurations = input.GAConfigurations;
        saConfigurations = input.SAConfigurations;
        bqeaConfigurations = input.BQEAConfigurations;
        mqeaConfigurations = input.MQEAConfigurations;
        timeLimits = input.timeLimits;
        threadCount = input.threadCount;
        runCount = input.problemCount;   

        problemSelection_ = input.problemSelection;
        imspConfigurations_ = input.IMSPconfigurations;
        mcpConfigurations_ = input.MCPConfigurations;
        txnspConfigurations_ = input.TxnSPConfigurations;
        tspConfigurations_ = input.TSPConfigurations;
        wmcpConfigurations_ = input.WMCPConfigurations;
        useExisting_ = input.useExisting;
        sstream_.setf(std::ios::fixed);
        sstream_.precision(2);
        sstream2_.setf(std::ios::fixed);
        sstream2_.precision(2);
        runProblems();
    }
}