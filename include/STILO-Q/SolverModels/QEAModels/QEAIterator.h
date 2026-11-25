#pragma once
#include "STILO-Q/ProblemModels/Problem.h"
#include "STILO-Q/Shared/Decode.h"
#include "STILO-Q/Shared/EvolvingParameter.h"
#include "STILO-Q/Shared/Parameters.h"
#include "STILO-Q/SolverModels/QEAModels/System.h"
#include "STILO-Q/SolverModels/QEAModels/QEAIteratorWrapper.h"

namespace QEVO
{
    template<
        template <class, bool> class GraphType,
        class ParticleType,        
        bool IsUnique,
        SolutionEncoding Encoding,
        AngleCalculationOperator AngleCalculation,
        bool GlobalBestRotation,
        bool IterationBestRotation,
        bool ModelBestRotation>
    class QEAIterator final : public QEAIteratorWrapper
    {
        using SystemClass = System<GraphType, ParticleType, IsUnique>;

    public:

        QEAIterator(QEASolverKey, Problem* problem, int populationSize, int measurementCount,
            int globalBestRotationCount, int iterationBestRotationCount, int modelBestRotationCount,
            double limitAngle, InitializationOperator initialization,
            std::vector<std::vector<std::vector<double>>>& heuristic, double baseGlobalBestAngle,
            double baseIterationBestAngle, double baseModelBestAngle, double heuristicInfluence)
            : problem_(problem),
              stateCount_(problem->getStateCount()),
              stringLength_(problem->getStringLength()),              
              populationSize_(populationSize),
              measurementCount_(measurementCount),
              globalBestRotationCount_(globalBestRotationCount),
              iterationBestRotationCount_(iterationBestRotationCount),
              modelBestRotationCount_(modelBestRotationCount),
              baseGlobalBestAngle_(baseGlobalBestAngle),
              baseIterationBestAngle_(baseIterationBestAngle),
              baseModelBestAngle_(baseModelBestAngle),
              costs_(populationSize, measurementCount),
              indices_(populationSize, measurementCount),              
              iterationCount_(0),
              scale_(problem->getScaleFactor())
        {
            if constexpr (Encoding == SolutionEncoding::Binary)
            {
                tempSolution_.resize(stringLength_);
                digitCount_ = ceil(log2(stateCount_));
                bstringLength_ = stringLength_ * digitCount_;
            }
            else if constexpr (Encoding == SolutionEncoding::Dense)
            {
                tempSolution_.resize(stringLength_);
                maxDigit_ = 1;

                for(int i = 2; i < stringLength_; i++)
                {
                    maxDigit_ *= i;
                }

                size_ = maxDigit_ * stringLength_;
                bstringLength_ = ceil(log2((double)size_));
                scratchBuffer_.resize(stringLength_);
            }
            else if constexpr (Encoding == SolutionEncoding::Integer)
            {
                bstringLength_ = stringLength_;
            }

            solutions_ = Tensor<int>(populationSize_, measurementCount_, bstringLength_);
            systems_.reserve(populationSize);

            for(int i = 0; i < populationSize; i++)
            {
                systems_.emplace_back(typename SystemClass::QEAIteratorKey{}, stateCount_, bstringLength_,
                    limitAngle, initialization, heuristic, heuristicInfluence);
            }

            if constexpr (GlobalBestRotation)
            {
                globalBestCosts_.assign(globalBestRotationCount_, DBL_MAX);
                globalBestSolutions_.resize(globalBestRotationCount_);

                for(int i = 0; i < globalBestRotationCount_; i++)
                {
                    globalBestSolutions_[i].resize(bstringLength_);
                }
            }
            else
            {
                globalBestCosts_.assign(1, DBL_MAX);
                globalBestSolutions_.resize(1);
                globalBestSolutions_[0].resize(bstringLength_);
            }
            
            if constexpr (IterationBestRotation)
            {
                iterationBestCosts_.assign(iterationBestRotationCount_, DBL_MAX);
                iterationBestRotationFirstIndices_.resize(iterationBestRotationCount_);
                iterationBestRotationSecondIndices_.resize(iterationBestRotationCount_);
            }
        }

        void iterate(QEASolverKey) override
        {
            if(IterationBestRotation)
            {
                iterationBestCosts_.assign(iterationBestRotationCount_, DBL_MAX);
            }

            for(int i = 0; i < populationSize_; i++)
            {
                costsRow_ = costs_[i];
                indicesRow_ = indices_[i];

                for(int j = 0; j < measurementCount_; j++)
                {
                    auto solution = solutions_.getRow(i, j);
                    systems_[i].measure(typename SystemClass::QEAIteratorKey{}, solution);
                    costsRow_[j] = evaluateSolution(solution);

                    int ind = findMinimum(costsRow_, indicesRow_, costsRow_[j], j);
                    shiftRightPartial<int>(indicesRow_, ind, j);
                    indicesRow_[ind] = j;
                }

                if constexpr (IterationBestRotation)
                {
                    int iterationInd = 0;
                    int localInd = 0;

                    while(iterationInd < iterationBestRotationCount_)
                    {
                        if(costsRow_[indicesRow_[localInd]] < iterationBestCosts_[iterationInd])
                        {
                            shiftRight<int>(iterationBestRotationFirstIndices_, iterationInd, iterationBestRotationCount_);
                            shiftRight<int>(iterationBestRotationSecondIndices_, iterationInd, iterationBestRotationCount_);
                            shiftRight<double>(iterationBestCosts_, iterationInd, iterationBestRotationCount_);
                            iterationBestRotationFirstIndices_[iterationInd] = i;
                            iterationBestRotationSecondIndices_[iterationInd] = indicesRow_[localInd];
                            iterationBestCosts_[iterationInd] = costsRow_[indicesRow_[localInd]];
                            localInd++;
                        }

                        iterationInd++;
                    }
                }

                if constexpr (GlobalBestRotation)
                {
                    int globalInd = 0;
                    int localInd = 0;

                    while(globalInd < globalBestRotationCount_)
                    {
                        if(costsRow_[indicesRow_[localInd]] < globalBestCosts_[globalInd])
                        {
                            shiftRight<double>(globalBestCosts_, globalInd, globalBestRotationCount_);
                            shiftListRight<int>(globalBestSolutions_, globalInd, globalBestRotationCount_);
                            globalBestCosts_[globalInd] = costsRow_[indicesRow_[localInd]];
                            copyAsymmetric(solutions_.getRow(i, indicesRow_[localInd]), globalBestSolutions_[globalInd], bstringLength_);
                            localInd++;
                        }

                        globalInd++;
                    }
                }

                if constexpr (!(GlobalBestRotation || IterationBestRotation))
                {
                    if (costsRow_[indicesRow_[0]] < globalBestCosts_[0])
                    {
                        globalBestCosts_[0] = costsRow_[indicesRow_[0]];
                        copy(solutions_.getRow(i, indicesRow_[0]), globalBestSolutions_[0], stringLength_);
                    }
                }
            }          

            if constexpr (ModelBestRotation)
            {
                applyModelBestRotations();
            }

            if constexpr (IterationBestRotation)
            {
                applyIterationBestRotations();
            }

            if constexpr (GlobalBestRotation)
            {
                applyGlobalBestRotations();
            }
            else if constexpr (IterationBestRotation)
            {
                if(iterationBestCosts_[0] < globalBestCosts_[0])
                {
                    globalBestCosts_[0] = iterationBestCosts_[0];
                    copyAsymmetric(solutions_.getRow(iterationBestRotationFirstIndices_[0], iterationBestRotationSecondIndices_[0]), globalBestSolutions_[0], stringLength_);
                }
            }

            iterationCount_++;
        }

        SolverOutput getOutput(QEASolverKey) override
        {
            return SolverOutput(globalBestCosts_[0], problem_->getValue(globalBestCosts_[0]), globalBestSolutions_[0], iterationCount_);
        }

    private:
        
        Problem* problem_;
        std::vector<System<GraphType, ParticleType, IsUnique>> systems_;        
        Tensor<int> solutions_;
        Matrix<double> costs_;
        Matrix<int> indices_;
        std::vector<std::vector<int>> globalBestSolutions_;
        std::vector<double> globalBestCosts_;
        std::vector<double> iterationBestCosts_;
        std::vector<int> tempSolution_;
        std::vector<int> scratchBuffer_;
        std::vector<int> iterationBestRotationFirstIndices_;
        std::vector<int> iterationBestRotationSecondIndices_;
        std::span<double> costsRow_;
        std::span<int> indicesRow_;
        LargeInt maxDigit_;
        LargeInt size_;
        double baseGlobalBestAngle_;
        double baseIterationBestAngle_;
        double baseModelBestAngle_;
        double scale_;
        int stringLength_;
        int stateCount_;
        int bstringLength_;
        int populationSize_;
        int measurementCount_;
        int globalBestRotationCount_;
        int iterationBestRotationCount_;
        int modelBestRotationCount_;
        int digitCount_;
        int iterationCount_;

        double evaluateSolution(std::span<const int> solution)
        {
            if constexpr (Encoding == SolutionEncoding::Binary)
            {
                fromBinaryToVector(solution, tempSolution_, digitCount_, stringLength_);
                return problem_->getCost(tempSolution_);
            }
            else if constexpr (Encoding == SolutionEncoding::Dense)
            {
                LargeInt index = fromBinaryToIndex(solution, bstringLength_);

                if(index >= size_)
                {
                    return DBL_MAX;
                }

                fromIndexToPermutation(index, maxDigit_, tempSolution_, scratchBuffer_, stringLength_);
                return problem_->getCost(tempSolution_);
            }
            else if constexpr (Encoding == SolutionEncoding::Integer)
            {
                return problem_->getCost(solution);
            }
        }

        double calculateModelBestRotationAngle(int solutionRank)
        {
            if constexpr (AngleCalculation == AngleCalculationOperator::Constant)
            {
                return baseModelBestAngle_;
            }
            else if constexpr (AngleCalculation == AngleCalculationOperator::CostBased)
            {
                return baseModelBestAngle_ * scale_ / costsRow_[indicesRow_[solutionRank]];
            }            
            else if constexpr (AngleCalculation == AngleCalculationOperator::RankBased)
            {
                return baseModelBestAngle_ * (double)(modelBestRotationCount_ - solutionRank) / modelBestRotationCount_;
            }
        }

        double calculateIterationBestRotationAngle(int solutionRank)
        {
            if constexpr (AngleCalculation == AngleCalculationOperator::Constant)
            {
                return baseIterationBestAngle_;
            }
            else if constexpr (AngleCalculation == AngleCalculationOperator::CostBased)
            {
                return baseIterationBestAngle_ * scale_ / iterationBestCosts_[solutionRank];
            }
            else if constexpr (AngleCalculation == AngleCalculationOperator::RankBased)
            {
                return baseIterationBestAngle_ * (iterationBestRotationCount_ - solutionRank) / iterationBestRotationCount_;
            }
        }

        double calculateGlobalBestRotationAngle(int solutionRank)
        {
            if constexpr (AngleCalculation == AngleCalculationOperator::Constant)
            {
                return baseGlobalBestAngle_;
            }
            else if constexpr (AngleCalculation == AngleCalculationOperator::CostBased)
            {
                return baseGlobalBestAngle_ * scale_ / globalBestCosts_[solutionRank];
            }
            
            else if constexpr (AngleCalculation == AngleCalculationOperator::RankBased)
            {
                return baseGlobalBestAngle_ * (globalBestRotationCount_ - solutionRank) / globalBestRotationCount_;
            }
        }

        void applyModelBestRotations()
        {
            for(int i = 0; i < populationSize_; i++)
            {
                indicesRow_ = indices_[i];

                if constexpr (AngleCalculation == AngleCalculationOperator::CostBased)
                {
                    costsRow_ = costs_[i];
                }
                
                for(int j = 0; j < modelBestRotationCount_; j++)
                {
                    auto solution = solutions_.getRow(i, indicesRow_[j]);
                    systems_[i].addRotation(typename SystemClass::QEAIteratorKey{},
                        calculateModelBestRotationAngle(j), solution);
                }
            }
        }

        void applyIterationBestRotations()
        {
            for(int i = 0; i < iterationBestRotationCount_; i++)
            {
                auto solution = solutions_.getRow(iterationBestRotationFirstIndices_[i],
                    iterationBestRotationSecondIndices_[i]);
                double angle = calculateIterationBestRotationAngle(i);

                for(int j = 0; j < populationSize_; j++)
                {
                    systems_[j].addRotation(typename SystemClass::QEAIteratorKey{}, angle, solution);
                }
            }  
        }

        void applyGlobalBestRotations()
        {
            for(int i = 0; i < globalBestRotationCount_; i++)
            {
                double angle = calculateGlobalBestRotationAngle(i);

                for(int j = 0; j < populationSize_; j++)
                {
                    systems_[j].addRotation(typename SystemClass::QEAIteratorKey{}, angle, globalBestSolutions_[i]);
                }
            }
        }
    };
}