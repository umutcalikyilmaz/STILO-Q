#pragma once
#include "STILOQ/SolverModels/QEAModels/Particle.h"

namespace STILOQ
{
    class BinaryParticle final : public Particle<BinaryParticle>
    {
    public:        

        BinaryParticle(SystemKey, double limitAngle)
            : Particle(2, limitAngle),
              sqrcoef_(0.5),
              rotation_(0) { }

        BinaryParticle(SystemKey, double limitAngle, const std::vector<double>& coefficients,
            double heuristicInfluence)
            : Particle(2, limitAngle),
              rotation_(0)
        {
            double tot = pow(coefficients[0], heuristicInfluence) + pow(coefficients[1], heuristicInfluence);
            sqrcoef_ = coefficients[0] / tot;
        }
        
        int measureExplicit(ParticleKey)
        {
            return measureBinary();
        }

        int measureExplicit(ParticleKey, const std::vector<int>& permitted, int pnum)
        {
            return measureBinary();
        }

        void addUpdateExplicit(ParticleKey, double angle, int direction)
        {
            needUpdate = true;

            if(direction == 0)
            {
                rotation_ += angle;
            }
            else
            {
                rotation_ -= angle;
            }        
        }

    private:
        
        double sqrcoef_;
        double rotation_;

        void applyUpdates()
        {
            double initAngle = acos(sqrt(sqrcoef_));
            double newAngle;
            newAngle = initAngle - rotation_;

            if(newAngle < limitAngle)
            {
                newAngle = limitAngle;
            }
            else if(newAngle > upperLimitAngle)
            {
                newAngle = upperLimitAngle;
            }
            
            sqrcoef_ = pow(cos(newAngle), 2);
            rotation_ = 0;
            needUpdate = false;
        }

        int measureBinary()
        {
            if(needUpdate)
            {
                applyUpdates();
            }

            double p = rnd.generate();
            return p < sqrcoef_ ? 0 : 1;
        }
    };
}