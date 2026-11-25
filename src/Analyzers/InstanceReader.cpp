#include "STILO-Q/Analyzers/InstanceReader.h"

namespace QEVO
{
    std::vector<std::tuple<std::unique_ptr<Problem>, std::string, double>> InstanceReader::readInstances(ProblemType pt, string instanceDirectory)
    {
        switch (pt)
        {            
        case ProblemType::IdenticalMachinesScheduling:
            readFilePtr_ = &InstanceReader::readIMSP;
            break;

        case ProblemType::MaxCut:
            readFilePtr_ = &InstanceReader::readMCP;
            break;

        case ProblemType::TransactionScheduling:
            readFilePtr_ = &InstanceReader::readTxnSP;
            break;

        case ProblemType::TravelingSalesperson:
            readFilePtr_ = &InstanceReader::readTSP;
            break;

        case ProblemType::WeightedMaxCut:
            readFilePtr_ = &InstanceReader::readWMCP;
            break;

        default:
            throw InvalidParameterException((char*)"Invalid problem type");
        }
        
        fstream file;
        std::vector<std::tuple<std::unique_ptr<Problem>, std::string, double>> res;

        for(const auto & entry : fs::directory_iterator(instanceDirectory))
        {
            if(entry.is_directory())
            {
                continue;
            }
            file.open(entry.path().string(), fstream::in);
            std::string instanceName = entry.path().filename().string();
            res.push_back((this->*readFilePtr_)(file, instanceName));
            file.close();
        }

        return res;
    }

    ProblemType InstanceReader::getProblemType()
    {
        return lastProbemType_;
    }

    tuple<std::unique_ptr<Problem>, std::string, double> InstanceReader::readIMSP(fstream& file, std::string& instanceName)
    {
        
        string line;
        std::vector<double> lengths;
        double opt = -1;
        int c = 0;
        int jn;
        int mn;

        while(getline(file, line))
        {
            int length = line.size();

            if(length > 0)
            {
                if(line[0] == 'p')
                {
                    int cur = 2;

                    for(int i = 2; i < length; i++)
                    {
                        if(line[i] == ' ')
                        {                            
                            jn = stoi(line.substr(2, i - 2));
                            cur = i + 1;
                            break;
                        }
                    }

                    mn = stoi(line.substr(cur, length - cur));
                    lengths.reserve(jn);
                }
                else if(line[0] == 'o')
                {
                    opt = stod(line.substr(2, length - 2));
                }
                else if(isdigit(line[0]))
                {
                    lengths.push_back(stod(line));
                    c++;
                }
            }
        }

        lastProbemType_ = ProblemType::IdenticalMachinesScheduling;        
        return std::make_tuple(std::make_unique<IdenticalMachinesSchedulingProblem>(jn, mn, lengths), instanceName, opt);
    }

    tuple<std::unique_ptr<Problem>, std::string, double> InstanceReader::readMCP(fstream& file, std::string& instanceName)
    {
        string line;
        int vn;
        int en;
        bool wmcp = false;
        double opt = -1;
        double minWeight = 0;
        std::vector<std::vector<uint8_t>> edges;
        std::vector<std::vector<double>> weights;

        while(getline(file, line))
        {
            int length = line.size();

            if(line[0] == 'p')
            {
                int cur = 2;

                for(int i = 2; i < length; i++)
                {
                    if(line[i] == ' ')
                    {
                        vn = stoi(line.substr(cur, i - cur));
                        cur = i + 1;
                        break;
                    }
                }

                en = stoi(line.substr(cur, length - cur));
                edges.resize(vn - 1);
                weights.resize(vn - 1);

                for(int i = 0; i < vn - 1; i++)
                {
                    edges[i].resize(i + 1);
                    weights[i].resize(i + 1);
                }
            }
            else if(line[0] == 'o')
            {
                opt = stod(line.substr(2, length - 2));
            }
            else if (isdigit(line[0]))
            {

                int cur = 0;
                int e1;
                int e2;
                bool first = true;
                
                double w;

                for(int i = 0; i < length; i++)
                {
                    if(line[i] == ' ')
                    {
                        if(first)
                        {
                            e1 = stoi(line.substr(cur, i - cur)) - 1;
                            cur = i + 1;
                            first = false;
                        }
                        else
                        {
                            e2 = stoi(line.substr(cur, i - cur)) - 1;
                            cur = i + 1;
                            break;
                        }
                    }
                }

                if(e1 > e2)
                {
                    e1--;
                }
                else
                {
                    double temp = e2 - 1;
                    e2 = e1;
                    e1 = temp;
                }

                w = stod(line.substr(cur, length - cur));
                edges[e1][e2] = 1;
                weights[e1][e2] = w;

                if(w != 1)
                {
                    wmcp = true;
                }
                
                if(w < minWeight)
                {
                    minWeight = w;
                }
            }
        }

        std::unique_ptr<Problem> resp;

        if(wmcp)
        {
            if(minWeight < 0)
            {
                for(int i = 0; i < vn - 1; i++)
                {
                    for(int j = 0; j <= i; j++)
                    {
                        weights[i][j] -= minWeight;
                    }
                }
            }

            resp = std::make_unique<WeightedMaxCutProblem>(vn, weights);

            
        }
        else
        {
            resp = std::make_unique<MaxCutProblem>(vn, edges);
        }

        lastProbemType_ = wmcp ? ProblemType::WeightedMaxCut : ProblemType::MaxCut;
        return std::make_tuple(move(resp), instanceName, opt);
    }

    tuple<std::unique_ptr<Problem>, std::string, double> InstanceReader::readTxnSP(fstream& file, std::string& instanceName)
    {
        string line;
        int c = 0;
        int n;
        int m;
        double opt = -1;
        std::vector<double> lengths;
        std::unique_ptr<Matrix<uint8_t>> conflicts;

        while (getline(file, line))
        {
            int length = line.size(); 

            if(line[0] == 'p')
            {
                int cur = 2;

                for(int i = 2; i < length; i++)
                {
                    if(line[i] == ' ')
                    {
                        n = stoi(line.substr(2, i - 2));
                        conflicts = std::make_unique<Matrix<uint8_t>>(n, n);
                        cur = i + 1;
                        break;
                    }
                }

                m = stoi(line.substr(cur, length - cur));
            }
            else if(line[0] == 'o')
            {
                opt = stod(line.substr(2, length - 2));
            }
            else if(isdigit(line[0]))
            {
                if(c < n)
                {
                    lengths.push_back(stod(line));
                }
                else
                {
                    int rc = c - n;
                    int cc = 0;

                    for(int i = 0; i < n; i++)
                    {
                        conflicts->getRow(rc)[i] = line[cc] == '1' ? 1 : 0;                        
                        cc += 2;
                    }
                }

                c++;
            }
        }       

        lastProbemType_ = ProblemType::TransactionScheduling;
        return std::make_tuple(std::make_unique<TransactionSchedulingProblem>(n, m, lengths, *conflicts), instanceName, opt);
    }

    tuple<std::unique_ptr<Problem>, std::string, double> InstanceReader::readTSP(fstream& file, std::string& instanceName)
    {
        string line;
        int cn;
        double opt = -1;
        std::unique_ptr<Matrix<double>> distances; 
        std::vector<double> xcoordinates;
        std::vector<double> ycoordinates;

        while(getline(file, line))
        {
            int length = line.size(); 

            if(line[0] == 'p')
            {
                cn = stoi(line.substr(2, length - 2));
                xcoordinates.resize(cn);
                ycoordinates.resize(cn);
            }
            else if(line[0] == 'o')
            {
                opt = stod(line.substr(2, length - 2));
            }
            else if(isdigit(line[0]))
            {
                int cur = 0;
                int ind;
                bool first = true;

                for(int i = 0; i < length; i++)
                {
                    if(line[i] == ' ')
                    {
                        if(first)
                        {
                            ind = stoi(line.substr(cur, i - cur)) - 1;
                            first = false;
                            cur = i + 1;
                        }
                        else
                        {
                            xcoordinates[ind] = stod(line.substr(cur, i - cur));
                            cur = i + 1;
                            break;
                        }
                    }
                }

                ycoordinates[ind] = stod(line.substr(cur, length - cur));
            }
        }

        distances = make_unique<Matrix<double>>(cn, cn);

        for(int i = 0; i < cn; i++)
        {
            auto distancesRow = distances->getRow(i);

            for(int j = 0; j < cn; j++)
            {
                distancesRow[j] = sqrt(pow(xcoordinates[i] - xcoordinates[j], 2) + pow(ycoordinates[i] - ycoordinates[j], 2));
            }
        }

        lastProbemType_ = ProblemType::TravelingSalesperson;
        return std::make_tuple(std::make_unique<TravelingSalespersonProblem>(cn, *distances), instanceName, opt);
    }

    tuple<std::unique_ptr<Problem>, std::string, double> InstanceReader::readWMCP(fstream& file, std::string& instanceName)
    {
        return readMCP(file, instanceName);
    }
}