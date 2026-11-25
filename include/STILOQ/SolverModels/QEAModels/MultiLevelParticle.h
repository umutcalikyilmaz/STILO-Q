#pragma once
#include "STILOQ/SolverModels/QEAModels/Particle.h"

namespace STILOQ
{
    class MultiLevelParticle final : public Particle<MultiLevelParticle>
    {
    public:

        MultiLevelParticle(SystemKey, int stateCount, double limitAngle)
            : Particle(stateCount, limitAngle),
              sqrcoef_(stateCount, 1 / (double)stateCount),
              rotate_(stateCount, 1),
              tempProb_(stateCount),
              probabilities_(stateCount),
              aliases_(stateCount),
              stateRnd_(0, stateCount - 1) { }

        MultiLevelParticle(SystemKey, int stateCount, double limitAngle, const std::vector<double>& coefficients,
        double heuristicInfluence)
            : Particle(stateCount, limitAngle),
              sqrcoef_(stateCount),
              rotate_(stateCount, 1),
              tempProb_(stateCount),
              probabilities_(stateCount),
              aliases_(stateCount),
              stateRnd_(0, stateCount - 1)
        {
            double tot = 0;            

            for (int i = 0; i < stateCount; i++)
            {   
                sqrcoef_[i] = pow(coefficients[i], heuristicInfluence);
                tot += sqrcoef_[i];
            }

            if(tot > 0)
            {
                for (int i = 0; i < stateCount; i++)
                {   
                    sqrcoef_[i] /= tot;
                }
            }
            else
            {
                for (int i = 0; i < stateCount; i++)
                {   
                    sqrcoef_[i] = 1 / (double)stateCount;
                }
            }
        }

        int measureExplicit(ParticleKey)
        {
            if(needUpdate)
            {
                formAliasTable();
            }

            int ind = stateRnd_.generate();            
            double p = rnd.generate();
            return p < probabilities_[ind] ? ind : aliases_[ind];
        }

        int measureExplicit(ParticleKey, const std::vector<int>& permitted, int pnum)
        {		
            if(needUpdate)
            {
                applyUpdates();
            }
            
            probabilities_[0] = sqrcoef_[permitted[0]];

            for (int i = 1; i < pnum; i++)
            {
                probabilities_[i] = sqrcoef_[permitted[i]] + probabilities_[i - 1];
            }
                
            double p = rnd.generate() * probabilities_[pnum - 1];

            if(p == 0)
            {
                stateRnd_.changeBounds(0, pnum - 1);
                return stateRnd_.generate();
            }

            return findElement<double>(probabilities_, p, pnum);
        }

        void addUpdateExplicit(ParticleKey, double angle, int direction)
        {
            int c = 0;

            for(const int& i : rotationDirections_)
            {
                if(i == direction)
                {
                    rotationAngles_[c] += angle;
                    return;
                }

                c++;
            }

            rotationDirections_.push_back(direction);
            rotationAngles_.push_back(angle);
            needUpdate = true;
        }

    private:

        UniformRandomIntGenerator stateRnd_;
        std::vector<double> sqrcoef_;
        std::vector<double> rotationAngles_;
        std::vector<double> probabilities_;   
        std::vector<double> tempProb_;     
        std::vector<int> rotationDirections_;
        std::vector<int> aliases_;
        std::vector<int> smallIndices_;
        std::vector<int> largeIndices_;
        std::vector<uint8_t> rotate_;		
           
        void applyUpdates()
        {
            bool reset = false;
            double val = 1;
            int ind = 0;

            for(double angle : rotationAngles_)
            {
                if(angle != 0.0)
                {
                    int direction = rotationDirections_[ind];
                    double initAngle;
                    double newAngle;
                    double c2;

                    rotate_[direction] = 0;

                    if(reset)
                    {
                        sqrcoef_[direction] = val;
                    }
                    else
                    {
                        sqrcoef_[direction] *= val;
                    }

                    if(sqrcoef_[direction] >= 1)
                    {
                        reset = true;
                        initAngle = 0;
                        c2 = 0;
                    }	
                    else
                    {
                        initAngle = acos(sqrt(sqrcoef_[direction]));
                        c2 = pow(sin(initAngle), 2);
                    }

                    newAngle = initAngle - angle;

                    if(newAngle < limitAngle)
                    {
                        newAngle = limitAngle;
                    }
                    else if(newAngle > upperLimitAngle)
                    {
                        newAngle = upperLimitAngle;
                    }
                    
                    sqrcoef_[direction] = pow(cos(newAngle), 2);
                    double c1 = pow(sin(newAngle), 2);

                    if(c2 == 0)
                    {
                        val = c1 / (stateCount - 1);

                        for(int i = 0; i < ind; i++)
                        {
                            sqrcoef_[rotationDirections_[i]] = val;
                        }
                    }	
                    else
                    {
                        double val2 = c1 / c2;
                        val *= val2;

                        for(int i = 0; i < ind; i++)
                        {
                            sqrcoef_[rotationDirections_[i]] *= val2;
                        }
                    }
                }
            
                ind++;
            }

            if(reset)
            {
                for(int i = 0; i < stateCount; i++)
                {
                    if(rotate_[i])
                    {
                        sqrcoef_[i] = val;					
                    }
                }
            }
            else
            {
                for(int i = 0; i < stateCount; i++)
                {
                    if(rotate_[i])
                    {
                        sqrcoef_[i] *= val;
                    }
                }
            }

            for(int dir : rotationDirections_)
            {
                rotate_[dir] = 1;
            }

            rotationAngles_.clear();
            rotationDirections_.clear();
            needUpdate = false;
        }

        void formAliasTable()
        {
            applyUpdates();

            double stdProb = 1 / (double)stateCount;

            for(int i = 0; i < stateCount; i++)
            {
                tempProb_[i] = sqrcoef_[i] - stdProb;
                probabilities_[i] = 1;

                if(tempProb_[i] < 0)
                {
                    smallIndices_.push_back(i);
                }
                else
                {
                    largeIndices_.push_back(i);
                }
            }

            while(!smallIndices_.empty() && !largeIndices_.empty())
            {
                int largeInd = largeIndices_.back();                    
                int smallInd = smallIndices_.back();
                smallIndices_.pop_back();

                aliases_[smallInd] = largeInd;
                probabilities_[smallInd] += tempProb_[smallInd] * this->stateCount;
                tempProb_[largeInd] += tempProb_[smallInd];

                if(tempProb_[largeInd] < 0)
                {
                    largeIndices_.pop_back();
                    smallIndices_.push_back(largeInd);
                }
                else if(tempProb_[largeInd] == 0)
                {
                    largeIndices_.pop_back();
                }
            }

            largeIndices_.clear();
            smallIndices_.clear();
        }
    };
}