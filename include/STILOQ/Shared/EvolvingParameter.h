#pragma once
#include "STILOQ/Shared/Parameters.h"

namespace STILOQ
{
    class EvolvingParameterWrapper
    {
    public:

        virtual double getValue() = 0;

        virtual void evolve() = 0;

        virtual ~EvolvingParameterWrapper() = default;
    };

    template <EvolvingParameterType type>
    class EvolvingParameter : public EvolvingParameterWrapper
    {
    public:

        EvolvingParameter(double initialValue, double parameter)
            : value_(initialValue),
              initialValue_(initialValue),
              parameter_(parameter),
              count_(1) { }
        
        double getValue() override
        {
            return value_;
        }

        void evolve() override
        {
            if constexpr (type == EvolvingParameterType::Exponential)
            {
                value_ *= parameter_;
            }
            else if constexpr (type == EvolvingParameterType::Slow)
            {
                value_ = initialValue_ / log2(1 + count_++);
            }
            if constexpr (type == EvolvingParameterType::Fast)
            {
                value_ = initialValue_ / count_++;
            }
        }

    private:

        double value_;
        double initialValue_;
        double parameter_;
        int count_;
    };    
}