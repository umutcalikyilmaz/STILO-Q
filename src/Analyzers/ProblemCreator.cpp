#include "STILOQ/Analyzers/ProblemCreator.h"

namespace STILOQ
{
    void ProblemCreator::openFile(string problem, string fileName, bool in)
    {
        const filesystem::path filePath = filesystem::path(std::getenv("HOME")) / ".stilo-q";

        if(in)
        {
            try
            {
                filesystem::create_directory(filePath);
                filesystem::create_directory(filePath / "problems");
                filesystem::create_directory(filePath / "problems" / problem);
                file_.open(filePath / "problems" / problem / fileName, fstream::in);
            }
            catch (const filesystem::filesystem_error& e)
            {
                cerr << "Cannot open problem file: " << e.what() << '\n';
            }
        }
        else
        {
            file_.open(filePath / "problems" / problem / fileName, fstream::app | fstream::out);
        }
    }

    std::unique_ptr<Problem> ProblemCreator::pullIMSP(int jobCount, int machineCount, const string& line)
    {
        int pos = 0;
        int size = line.length();
        std::vector<double> costs;

        for(int i = 0; i < jobCount - 1; i++)
        {
            costs.push_back(stod(parseNext(line, pos, size)));
        }

        costs.push_back(stod(parseLast(line, pos, size)));
        return std::make_unique<IdenticalMachinesSchedulingProblem>(jobCount, machineCount, costs);
    }

    std::unique_ptr<Problem> ProblemCreator::pushIMSP(int jobCount, int machineCount,
        ProbabilityDistribution lengthDistribution, double distributionParanmeter1,
        double distributionParameter2)
    {
        auto res = std::make_unique<IdenticalMachinesSchedulingProblem>(jobCount, machineCount,
            lengthDistribution, distributionParanmeter1, distributionParameter2);
        stringstream sstream;

        for(int i = 0; i < jobCount; i++)
        {
            sstream << to_string(res->lengths_[i]) << ",";
        }

        string line = sstream.str();
        line.pop_back();
        file_ << line << "\n";
        file_.flush();
        return res;
    }

    std::unique_ptr<Problem> ProblemCreator::pullMCP(int verticeCount, const string& line)
    {
        int pos = 0;
        int size = line.length();
        std::vector<std::vector<uint8_t>> edges(verticeCount - 1);

        for(int i = 0; i < verticeCount - 1; i++)
        {
            for(int j = 0; j <= i; j++)
            {
                if(i == verticeCount - 2 && j == i)
                {
                    break;
                }

                edges[i].push_back(stoi(parseNext(line, pos, size)));
            }
        }

        edges[verticeCount - 2].push_back(stoi(parseLast(line, pos, size)));
        return std::make_unique<MaxCutProblem>(verticeCount, edges);
    }

    std::unique_ptr<Problem> ProblemCreator::pushMCP(int verticeCount, double edgePropensity)
    {
        auto res = std::make_unique<MaxCutProblem>(verticeCount, edgePropensity);
        stringstream sstream;

        for(int i = 0; i < verticeCount - 1; i++)
        {
            for(int j = 0; j <= i; j++)
            {
                sstream << static_cast<int>(res->edges_[i][j]) << ",";
            }
        }

        string line = sstream.str();
        line.pop_back();
        file_ << line << "\n";
        file_.flush();
        return res;
    }

    std::unique_ptr<Problem> ProblemCreator::pullTxnSP(int jobCount, int machineCount, const string& line)
    {
        int pos = 0;
        int size = line.length();
        std::vector<double> lengths;
        Matrix<uint8_t> conflicts(jobCount, jobCount);

        for(int i = 0; i < jobCount; i++)
        {
            lengths.push_back(stod(parseNext(line, pos, size)));
        }

        for(int i = 0; i < jobCount; i++)
        {            
            for(int j = 0; j < jobCount; j++)
            {
                if(i == jobCount - 1 && j == jobCount - 1)
                {
                    break;
                }

                conflicts[i][j] = stoi(parseNext(line, pos, size));
            }
        }

        conflicts[jobCount - 1][jobCount - 1] = stoi(parseLast(line, pos, size));
        return std::make_unique<TransactionSchedulingProblem>(jobCount, machineCount, lengths, conflicts);
    }

    std::unique_ptr<Problem> ProblemCreator::pushTxnSP(int jobCount, int machineCount, double conflictPropensity,
        ProbabilityDistribution lengthDistribution, double distributionParameter1,
        double distributionParameter2)
    {
        auto res = std::make_unique<TransactionSchedulingProblem>(jobCount, machineCount, conflictPropensity,
            lengthDistribution, distributionParameter1, distributionParameter2);
        stringstream sstream;

        for(int i = 0; i < jobCount; i++)
        {
            sstream << to_string(res->lengths_[i]) << ",";
        }

        for(int i = 0; i < jobCount; i++)
        {
            for(int j = 0; j < jobCount; j++)
            {
                sstream << static_cast<int>(res->conflicts_[i][j]) << ",";
            }
        }

        string line = sstream.str();
        line.pop_back();
        file_ << line << "\n";
        file_.flush();
        return res;
    }

    std::unique_ptr<Problem> ProblemCreator::pullTSP(int cityCount, const string& line)
    {
        int pos = 0;
        int size = line.length();
        Matrix<double> distances(cityCount, cityCount);

        for(int i = 0; i < cityCount; i++)
        {
            for(int j = 0; j < cityCount; j++)
            {
                if(i == cityCount - 1 && j == cityCount - 1)
                {
                    break;
                }

                distances[i][j] = stod(parseNext(line, pos, size));
            }
        }

        distances[cityCount - 1][cityCount - 1] = stod(parseLast(line, pos, size));
        return std::make_unique<TravelingSalespersonProblem>(cityCount, distances);
    }

    std::unique_ptr<Problem> ProblemCreator::pushTSP(int cityCount, ProbabilityDistribution distanceDistribution,
        double distributionParameter1, double distributionParameter2)
    {
        auto res = std::make_unique<TravelingSalespersonProblem>(cityCount, distanceDistribution,
            distributionParameter1, distributionParameter2);
        stringstream sstream;

        for(int i = 0; i < cityCount; i++)
        {
            for(int j = 0; j < cityCount; j++)
            {
                sstream << to_string(res->distances_[i][j]) << ",";
            }            
        }

        string line = sstream.str();
        line.pop_back();
        file_ << line << "\n";
        file_.flush();
        return res;
    }

    std::unique_ptr<Problem> ProblemCreator::pullWMCP(int verticeCount, const string& line)
    {
        int pos = 0;
        int size = line.length();
        std::vector<std::vector<double>> weights(verticeCount - 1);

        for(int i = 0; i < verticeCount - 1; i++)
        {
            for(int j = 0; j <= i; j++)
            {
                if(i == verticeCount - 2 && j == i)
                {
                    break;
                }

                weights[i].push_back(stod(parseNext(line, pos, size)));
            }
        }

        weights[verticeCount - 2].push_back(stod(parseLast(line, pos, size)));
        return std::make_unique<WeightedMaxCutProblem>(verticeCount, weights);
    }

    std::unique_ptr<Problem> ProblemCreator::pushWMCP(int verticeCount, ProbabilityDistribution weightDistribution,
        double distributionParameter1, double distributionParameter2)
    {
        auto res = std::make_unique<WeightedMaxCutProblem>(verticeCount, weightDistribution,
            distributionParameter1, distributionParameter2);
        stringstream sstream;

        for(int i = 0; i < verticeCount - 1; i++)
        {
            for(int j = 0; j <= i; j++)
            {
                sstream << to_string(res->weights_[i][j]) << ",";
            }            
        }

        string line = sstream.str();
        line.pop_back();
        file_ << line << "\n";
        file_.flush();
        return res;
    }

    std::vector<std::unique_ptr<Problem>> ProblemCreator::createIMSP(int problemCount, bool useExisting,
        int jobCount, int machineCount, ProbabilityDistribution lengthDistribution,
        double distributionParanmeter1, double distributionParameter2)
    {
        std::vector<std::unique_ptr<Problem>> res;   

        if(useExisting)
        {
            stringstream sstream;
            sstream.setf(std::ios::fixed);
            sstream.precision(2);
            sstream << "IMSP_" << jobCount << "_" << machineCount << "_" << static_cast<int>(lengthDistribution)
                << "_" << distributionParanmeter1 << "_" << distributionParameter2 << ".csv";        
            openFile("IMSP", sstream.str(), true);
            string line;
            int c = 0;

            while(getline(file_, line) && c < problemCount)
            {
                res.emplace_back(pullIMSP(jobCount, machineCount, line));
                c++;
            }

            file_.close();
            openFile("IMSP", sstream.str(), false);

            for(int i = c; i < problemCount; i++)
            {
                res.emplace_back(pushIMSP(jobCount, machineCount, lengthDistribution, 
                    distributionParanmeter1, distributionParameter2));
            }

            file_.close();
        }
        else
        {
            for(int i = 0; i < problemCount; i++)
            {
                res.emplace_back(std::make_unique<IdenticalMachinesSchedulingProblem>(jobCount,
                    machineCount, lengthDistribution, distributionParanmeter1, distributionParameter2));
            }
        }
        
        return res;
    }

    std::vector<std::unique_ptr<Problem>> ProblemCreator::createMCP(int problemCount, bool useExisting,
        int verticeCount, double edgePropensity)
    {
        std::vector<std::unique_ptr<Problem>> res;          

        if(useExisting)
        {
            stringstream sstream;
            sstream.setf(std::ios::fixed);
            sstream.precision(2);
            sstream << "MCP_" << verticeCount << "_" << edgePropensity << ".csv";
            openFile("MCP", sstream.str(), true);
            string line;
            int c = 0;

            while(getline(file_, line) && c < problemCount)
            {
                res.emplace_back(pullMCP(verticeCount, line));
                c++;
            }

            file_.close();
            openFile("MCP", sstream.str(), false);

            for(int i = c; i < problemCount; i++)
            {
                res.emplace_back(pushMCP(verticeCount, edgePropensity));
            }

            file_.close();
        }
        else
        {
            for(int i = 0; i < problemCount; i++)
            {
                res.emplace_back(std::make_unique<MaxCutProblem>(verticeCount, edgePropensity));
            }
        }
        
        return res;
    }

    std::vector<std::unique_ptr<Problem>> ProblemCreator::createTxnSP(int problemCount, bool useExisting,
        int jobCount, int machineCount, double conflictPropensity, ProbabilityDistribution lengthDistribution,
        double distributionParameter1, double distributionParameter2)
    {
        std::vector<std::unique_ptr<Problem>> res;
        
        if(useExisting)
        {
            stringstream sstream;
            sstream.setf(std::ios::fixed);
            sstream.precision(2);
            sstream << "TxnSP_" << jobCount << "_" << machineCount << "_" << conflictPropensity << "_" <<
                static_cast<int>(lengthDistribution) << "_" << distributionParameter1 << "_" <<
                distributionParameter2 << ".csv";
            openFile("TxnSP", sstream.str(), true);
            string line;
            int c = 0;

            while(getline(file_, line) && c < problemCount)
            {
                res.emplace_back(pullTxnSP(jobCount, machineCount, line));
                c++;
            }

            file_.close();
            openFile("TxnSP", sstream.str(), false);

            for(int i = c; i < problemCount; i++)
            {
                res.emplace_back(pushTxnSP(jobCount, machineCount, conflictPropensity, lengthDistribution,
                    distributionParameter1, distributionParameter2));
            }

            file_.close();
        }
        else
        {
            for(int i = 0; i < problemCount; i++)
            {
                res.emplace_back(std::make_unique<TransactionSchedulingProblem>(jobCount, machineCount,
                    conflictPropensity, lengthDistribution, distributionParameter1, distributionParameter2));
            }
        }

        return res;
    }

    std::vector<std::unique_ptr<Problem>> ProblemCreator::createTSP(int problemCount, bool useExisting,
        int cityCount, ProbabilityDistribution distanceDistribution, double distributionParameter1,
        double distributionParameter2)
    {
        std::vector<std::unique_ptr<Problem>> res;
        
        if(useExisting)
        {
            stringstream sstream;
            sstream.setf(std::ios::fixed);
            sstream.precision(2);
            sstream << "TSP_" << cityCount << "_" << static_cast<int>(distanceDistribution) << "_"
                << distributionParameter1 << "_" << distributionParameter2 << ".csv";
            openFile("TSP", sstream.str(), true);
            string line;
            int c = 0;

            while(getline(file_, line) && c < problemCount)
            {
                res.emplace_back(pullTSP(cityCount, line));
                c++;
            }

            file_.close();
            openFile("TSP", sstream.str(), false);

            for(int i = c; i < problemCount; i++)
            {
                res.emplace_back(pushTSP(cityCount, distanceDistribution, distributionParameter1,
                    distributionParameter2));
            }

            file_.close();
        }
        else
        {
            for(int i = 0; i < problemCount; i++)
            {
                res.emplace_back(std::make_unique<TravelingSalespersonProblem>(cityCount,
                    distanceDistribution, distributionParameter1, distributionParameter2));
            }
        }

        return res;
    }

    std::vector<std::unique_ptr<Problem>> ProblemCreator::createWMCP(int problemCount, bool useExisting,
        int verticeCount, ProbabilityDistribution weightDistribution, double distributionParameter1,
        double distributionParameter2)
    {
        std::vector<std::unique_ptr<Problem>> res;        

        if(useExisting)
        {
            stringstream sstream;
            sstream.setf(std::ios::fixed);
            sstream.precision(2);
            sstream << "WMCP_" << verticeCount << "_" << static_cast<int>(weightDistribution) << "_" <<
                distributionParameter1 << "_" << distributionParameter2 << ".csv";
            openFile("WMCP", sstream.str(), true);
            string line;
            int c = 0;

            while(getline(file_, line) && c < problemCount)
            {
                res.emplace_back(pullWMCP(verticeCount, line));
                c++;
            }

            file_.close();
            openFile("WMCP", sstream.str(), false);

            for(int i = c; i < problemCount; i++)
            {
                res.emplace_back(pushWMCP(verticeCount, weightDistribution, distributionParameter1,
                    distributionParameter2));
            }

            file_.close();
        }
        else
        {
            for(int i = 0; i < problemCount; i++)
            {
                res.emplace_back(std::make_unique<WeightedMaxCutProblem>(verticeCount, weightDistribution,
                    distributionParameter1, distributionParameter2));
            }
        }

        return res;
    }
}