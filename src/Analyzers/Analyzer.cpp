#include "STILOQ/Analyzers/Analyzer.h"

namespace STILOQ
{
    void Analyzer::createJobs()
    {
        bestACOConfigurations_.clear();
        bestGAConfigurations_.clear();
        bestSAConfigurations_.clear();
        bestBQEAConfigurations_.clear();
        bestMQEAConfigurations_.clear();

        double val = minimization ? DBL_MAX : -DBL_MAX;

        if(solverSelection.analyzeACO)
        {
            auto jobVector = jobCreator_.createACOJobs(acoConfigurations);

            for(int i = 0; i < jobVector.size(); i++)
            {
                jobQueue.push(move(jobVector[i]));
            }

            for(auto tl : timeLimits)
            {
                bestACOConfigurations_.try_emplace(tl, std::make_tuple(val, 0, 0, 0, ACOConfiguration()));
            }
        }

        if(solverSelection.analyzeGA)
        {
            auto jobVector = jobCreator_.createGAJobs(gaConfigurations);

            for(int i = 0; i < jobVector.size(); i++)
            {
                jobQueue.push(move(jobVector[i]));
            }

            for(auto tl : timeLimits)
            {         
                bestGAConfigurations_.try_emplace(tl, std::make_tuple(val, 0, 0, 0, GAConfiguration()));
            }
        }

        if(solverSelection.analyzeSA)
        {
            auto jobVector = jobCreator_.createSAJobs(saConfigurations);

            for(int i = 0; i < jobVector.size(); i++)
            {
                jobQueue.push(move(jobVector[i]));
            }

            for(auto tl : timeLimits)
            {
                bestSAConfigurations_.try_emplace(tl, std::make_tuple(val, 0, 0, 0, SAConfiguration()));
            }
        }

        if(solverSelection.analyzeBQEA)
        {
            auto jobVector = jobCreator_.createQEAJobs(bqeaConfigurations);

            for(int i = 0; i < jobVector.size(); i++)
            {
                jobVector[i]->solverType = SolverType::BinaryQuantumEvolutionaryAlgorithm;
                jobQueue.push(move(jobVector[i]));
            }

            for(auto tl : timeLimits)
            {
                bestBQEAConfigurations_.try_emplace(tl, std::make_tuple(val, 0, 0, 0, QEAConfiguration()));
            }
        }

        if(solverSelection.analyzeMQEA)
        {
            auto jobVector = jobCreator_.createQEAJobs(mqeaConfigurations);

            for(int i = 0; i < jobVector.size(); i++)
            {
                jobQueue.push(move(jobVector[i]));
            }

            for(auto tl : timeLimits)
            {
                bestMQEAConfigurations_.try_emplace(tl, std::make_tuple(val, 0, 0, 0, QEAConfiguration()));
            }
        }
    }

    std::unique_ptr<Solver> Analyzer::createSolver(SolverType solverType)
    {
        switch (solverType)
        {
        case SolverType::AntColonyOptimization:
            return std::make_unique<ACOSolver>();

        case SolverType::GeneticAlgorithm:
            return std::make_unique<GASolver>();

        case SolverType::SimulatedAnnealing:
            return std::make_unique<SASolver>();

        case SolverType::BinaryQuantumEvolutionaryAlgorithm:
            return std::make_unique<BQEASolver>();

        case SolverType::MultiLevelQuantumEvolutionaryAlgorithm:
            return std::make_unique<MQEASolver>();
        
        default:
            throw InvalidParameterException((char*)"Invalid solver type");
        }
    }

    bool Analyzer::pullJob(std::unique_ptr<Job>& job)
    {
        std::lock_guard<mutex> lock(jobQueueLock_);

        if(jobQueue.empty())
        {
            return false;
        }

        job = move(jobQueue.front());
        jobQueue.pop();
        return true;
    }

    void Analyzer::pushResult(JobResult result)
    {
        std::lock_guard<mutex> lock(resultQueueLock_);
        resultQueue_.push(move(result));
        resultQueueCondition_.notify_one();   
    }

    void Analyzer::initializeJob(Job* job, JobResult& result)
    {
        result.input = job->input;
        result.solverType = job->solverType;
        job->input.timeLimits = timeLimits;

        for(auto tl : timeLimits)
        {
            JobOutput res;
            res.averageCost = 0;
            res.averageIterationCount = 0;
            res.averageTime = 0;
            res.successRate = 0;

            result.results.try_emplace(tl, res);               
        }
    }

    void Analyzer::updateJobResult(const SolverAnalysisOutput& analysisOutput, JobResult& result, bool minimum)
    {
        for(auto tl : timeLimits)
        {
            auto& out = analysisOutput.results.at(tl);
            auto& res = result.results.at(tl);

            if(out != NULL)
            {
                res.successRate++;
                res.averageCost += minimum ? out->cost : out->value;
                res.averageIterationCount += out->iterationCount;
                res.averageTime += out->executionTime.count();
            }
        }
    }

    void Analyzer::finalizeJobResult(JobResult& result, int count)
    {
        for(auto tl : timeLimits)
        {
            auto& res = result.results.at(tl);

            if(res.successRate > 0)
            {
                res.averageCost /= res.successRate;
                res.averageIterationCount /= res.successRate;
                res.averageTime /= res.successRate;
                res.successRate /= count;
            }
        }
    }

    void Analyzer::displayResult(int& batchCount)
    {
        if(batchCount == threadCount)
        {
            std::lock_guard<std::mutex> lock(displayLock_);
            completed_+= threadCount;
            double percent = (double)completed_ / (double)jobCount_;
            std::cout << '\r' << std::fixed << std::setprecision(2) << 100.0 * percent << "% completed"
                << std::flush;
            batchCount = 0;
        }
        
    }

    void Analyzer::displayFinal(int batchCount)
    {
        std::lock_guard<std::mutex> lock(displayLock_);
        completed_ += batchCount;
        double percent = (double)completed_ / (double)jobCount_;
        std::cout << '\r' << std::fixed << std::setprecision(2) << 100.0 * percent << "% completed"
            << std::flush;

        completedThreadCount_++;
        resultQueueCondition_.notify_one();
    }

    bool Analyzer::compareObjectiveValues(double currentValue, double newValue)
    {
        return minimization ? currentValue > newValue : currentValue < newValue;
    }

    void Analyzer::runAnalyzerThread()
    {
        while(completedThreadCount_ != threadCount)
        {
            JobResult jobResult;

            {
                unique_lock<mutex> lock(resultQueueLock_);

                resultQueueCondition_.wait(lock, [this]
                {
                    return !resultQueue_.empty() || completedThreadCount_ == threadCount;
                });

                if(resultQueue_.empty())
                {
                    break;
                }
    
                jobResult = move(resultQueue_.front());
                resultQueue_.pop();
            }
            
            for(auto tl : timeLimits)
            {
                auto& output = jobResult.results.at(tl);
                
                if(output.successRate == 1)
                {
                    switch (jobResult.solverType)
                    {
                    case SolverType::AntColonyOptimization:
                        if(compareObjectiveValues(get<0>(bestACOConfigurations_.at(tl)), output.averageCost))
                        {
                            bestACOConfigurations_.at(tl) = std::make_tuple(output.averageCost,
                                output.averageTime, output.averageIterationCount, output.successRate,
                                jobResult.input.ACOConfig);
                        }
                        break;
    
                    case SolverType::GeneticAlgorithm:
                        if(compareObjectiveValues(get<0>(bestGAConfigurations_.at(tl)), output.averageCost))
                        {
                            bestGAConfigurations_.at(tl) = std::make_tuple(output.averageCost,
                                output.averageTime, output.averageIterationCount, output.successRate,
                                jobResult.input.GAConfig);
                        }
                        break;
    
                    case SolverType::SimulatedAnnealing:
                        if(compareObjectiveValues(get<0>(bestSAConfigurations_.at(tl)), output.averageCost))
                        {
                            bestSAConfigurations_.at(tl) = std::make_tuple(output.averageCost,
                                output.averageTime, output.averageIterationCount, output.successRate,
                                jobResult.input.SAConfig);
                        }
                        break;

                    case SolverType::BinaryQuantumEvolutionaryAlgorithm:
                        if(compareObjectiveValues(get<0>(bestBQEAConfigurations_.at(tl)), output.averageCost))
                        {
                            bestBQEAConfigurations_.at(tl) = std::make_tuple(output.averageCost,
                                output.averageTime, output.averageIterationCount, output.successRate,
                                jobResult.input.QEAConfig);
                        }
                        break;

                    case SolverType::MultiLevelQuantumEvolutionaryAlgorithm:
                        if(compareObjectiveValues(get<0>(bestMQEAConfigurations_.at(tl)), output.averageCost))
                        {
                            bestMQEAConfigurations_.at(tl) = std::make_tuple(output.averageCost,
                                output.averageTime, output.averageIterationCount, output.successRate,
                                jobResult.input.QEAConfig);
                        }
                        break;
                    
                    default:
                        throw InvalidParameterException((char*)"Invalid enum value");
                    }
                }
            }
        
            writeQueue_.push(move(jobResult));
        }
    }

    void Analyzer::initializeRun()
    {
        completed_ = 0;
        jobCount_ = jobQueue.size();
        completedThreadCount_ = 0;

        cout << "Total Count of Configurations: " << to_string(jobCount_) << "\n";
    }

    std::string Analyzer::getACOString(const ACOConfiguration& acoConfig)
    {
        stringstream sstream;
        sstream.precision(2);
        sstream << static_cast<int>(acoConfig.graphStructure) << "," <<
        static_cast<int>(acoConfig.tourSelectionOperator) << "," <<
        static_cast<int>(acoConfig.pheromoneCalculationOperator) << "," <<
        static_cast<int>(acoConfig.pheromoneUpdateOperator) << "," <<
        acoConfig.antCount << "," << acoConfig.evaporationParameter << "," << acoConfig.pheromoneInfluence
        << "," << acoConfig.heuristicInfluence << "," << acoConfig.initialPheromone << "," <<
        acoConfig.pheromoneConstant << "," << acoConfig.localDecayParameter << "," << acoConfig.eliteCount;
        return sstream.str();
    }

    std::string Analyzer::getGAString(const GAConfiguration& gaConfig)
    {
        stringstream sstream;
        sstream.precision(2);
        sstream << static_cast<int>(gaConfig.selectionOperator) << "," <<
        gaConfig.cycleCrossoverCoefficient << "," << gaConfig.kpointCrossoverCoefficient << "," <<
        gaConfig.OXCrossoverCoefficient << "," << gaConfig.PMXCrossoverCoefficient << "," <<
        gaConfig.uniformCrossoverCoefficient << "," << gaConfig.pointMutationCoefficient << "," <<
        gaConfig.insertMutationCoefficient << "," << gaConfig.invertMutationCoefficient << "," <<
        gaConfig.swapMutationCoefficient << "," << gaConfig.populationSize << "," << gaConfig.matingCount
        << "," << gaConfig.mutationProbability << "," << gaConfig.eliteCount << "," << gaConfig.k
        << "," << gaConfig.pointMutationProbability << "," << gaConfig.tournamentSize;
        return sstream.str();
    }

    std::string Analyzer::getSAString(const SAConfiguration& saConfig)
    {
        stringstream sstream;
        sstream.precision(2);
        sstream << static_cast<int>(saConfig.coolingSchedule) << "," <<
        static_cast<int>(saConfig.distanceCalculationOperator) << "," << saConfig.insertMoveCoefficient
        << "," << saConfig.invertMoveCoefficient << "," << saConfig.pointMoveCoefficient << "," <<
        saConfig.swapMoveCoefficient << "," << saConfig.initialTemperature << "," <<
        saConfig.minimumNeighborhoodSize << "," << saConfig.maximumNeighborhoodSize << "," <<
        to_string(saConfig.coolingParameter);
        return sstream.str();
    }

    std::string Analyzer::getQEAString(const QEAConfiguration& qeaConfig)
    {
        stringstream sstream;
        sstream.precision(2);
        sstream << static_cast<int>(qeaConfig.graphStructure) << "," <<
        static_cast<int>(qeaConfig.initializationType) << "," <<
        static_cast<int>(qeaConfig.angleCalculationOperator) << "," <<
        qeaConfig.populationSize << "," << qeaConfig.measurementCount << "," <<
        qeaConfig.globalBestRotationCount << "," << qeaConfig.baseGlobalBestAngle
        << "," << qeaConfig.iterationBestRotationCount << "," <<
        qeaConfig.baseIterationBestAngle << "," << qeaConfig.modelBestRotationCount << "," <<
        qeaConfig.baseModelBestAngle << "," << qeaConfig.limitAngle << "," << qeaConfig.heuristicInfluence;
        return sstream.str();
    }

    void Analyzer::printAnalysisResults(const std::string& analysisType, const std::string& problemClass,
        const std::string& instanceClass)
    {
        const filesystem::path filePath = std::filesystem::path(std::getenv("HOME")) / ".stilo-q";
        std::fstream file;

        try
        {
            filesystem::create_directory(filePath);
            filesystem::create_directory(filePath / analysisType);
            filesystem::create_directory(filePath / analysisType / "analysis");
            filesystem::create_directory(filePath / analysisType / "analysis" / problemClass);
            filesystem::create_directory(filePath / analysisType / "analysis" / problemClass / instanceClass);
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            cerr << "Cannot open result file: " << e.what() << '\n';
        }

        if(!bestACOConfigurations_.empty())
        {
            try
            {
                file.open(filePath / analysisType / "analysis" / problemClass / instanceClass / "ACO.csv", fstream::trunc | fstream::out);
            }
            catch (const std::filesystem::filesystem_error& e)
            {
                cerr << "Cannot open result file: " << e.what() << '\n';
            }

            for(auto tl : timeLimits)
            {
                auto& acoTuple = bestACOConfigurations_.at(tl);
                file << tl.count() << "," << "ACO," << std::to_string(runCount) << "," << 
                std::to_string(get<3>(acoTuple)) << "," << std::to_string(get<0>(acoTuple)) << "," <<
                std::to_string(get<1>(acoTuple)) << "," << std::to_string(get<2>(acoTuple)) << "," <<
                getACOString(get<4>(acoTuple)) << "\n";
            }

            file.flush();
            file.close();
        }

        if(!bestGAConfigurations_.empty())
        {
            try
            {
                file.open(filePath / analysisType / "analysis" / problemClass / instanceClass / "GA.csv", fstream::trunc | fstream::out);
            }
            catch (const std::filesystem::filesystem_error& e)
            {
                cerr << "Cannot open result file: " << e.what() << '\n';
            }

            for(auto tl : timeLimits)
            {
                auto& gaTuple = bestGAConfigurations_.at(tl);
                file << tl.count() << "," << "GA," << std::to_string(runCount) << "," << 
                std::to_string(get<3>(gaTuple)) << "," << std::to_string(get<0>(gaTuple)) << "," <<
                std::to_string(get<1>(gaTuple)) << "," << std::to_string(get<2>(gaTuple)) << "," <<
                getGAString(get<4>(gaTuple)) << "\n";
            }

            file.flush();
            file.close();
        }

        if(!bestSAConfigurations_.empty())
        {
            try
            {
                file.open(filePath / analysisType / "analysis" / problemClass / instanceClass / "SA.csv", fstream::trunc | fstream::out);
            }
            catch (const std::filesystem::filesystem_error& e)
            {
                cerr << "Cannot open result file: " << e.what() << '\n';
            }

            for(auto tl : timeLimits)
            {
                auto& saTuple = bestSAConfigurations_.at(tl);
                file << tl.count() << "," << "SA," << std::to_string(runCount) << "," << 
                std::to_string(get<3>(saTuple)) << "," << std::to_string(get<0>(saTuple)) << "," <<
                std::to_string(get<1>(saTuple)) << "," << std::to_string(get<2>(saTuple)) << "," <<
                getSAString(get<4>(saTuple)) << "\n";
            }

            file.flush();
            file.close();
        }

        if(!bestBQEAConfigurations_.empty())
        {
            try
            {
                file.open(filePath / analysisType / "analysis" / problemClass / instanceClass / "BQEA.csv", fstream::trunc | fstream::out);
            }
            catch (const std::filesystem::filesystem_error& e)
            {
                cerr << "Cannot open result file: " << e.what() << '\n';
            }

            for(auto tl : timeLimits)
            {
                auto& bqeaTuple = bestBQEAConfigurations_.at(tl);
                file << tl.count() << "," << "BQEA," << std::to_string(runCount) << "," << 
                std::to_string(get<3>(bqeaTuple)) << "," << std::to_string(get<0>(bqeaTuple)) << "," <<
                std::to_string(get<1>(bqeaTuple)) << "," << std::to_string(get<2>(bqeaTuple)) << "," <<
                getQEAString(get<4>(bqeaTuple)) << "\n";
            }

            file.flush();
            file.close();
        }

        if(!bestMQEAConfigurations_.empty())
        {
            try
            {
                file.open(filePath / analysisType / "analysis" / problemClass / instanceClass / "MQEA.csv", fstream::trunc | fstream::out);
            }
            catch (const std::filesystem::filesystem_error& e)
            {
                cerr << "Cannot open result file: " << e.what() << '\n';
            }

            for(auto tl : timeLimits)
            {
                auto& mqeaTuple = bestMQEAConfigurations_.at(tl);
                file << tl.count() << "," << "MQEA," << std::to_string(runCount) << "," << 
                std::to_string(get<3>(mqeaTuple)) << "," << std::to_string(get<0>(mqeaTuple)) << "," <<
                std::to_string(get<1>(mqeaTuple)) << "," << std::to_string(get<2>(mqeaTuple)) << "," <<
                getQEAString(get<4>(mqeaTuple)) << "\n";
            }

            file.flush();
            file.close();
        }
    }
}