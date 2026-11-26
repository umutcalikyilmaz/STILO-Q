#pragma once
#include "STILOQ/SolverModels/GraphModels/MultiStageGraph.h"
#include "STILOQ/SolverModels/GraphModels/SingleStageGraph.h"
#include "STILOQ/SolverModels/GraphModels/SequentialGraph.h"
#include "STILOQ/SolverModels/QEAModels/BinaryParticle.h"
#include "STILOQ/SolverModels/QEAModels/MultiLevelParticle.h"

namespace STILOQ
{
    template<
        template <class, bool> class GraphType,
        class ParticleType,
        bool IsUnique>
    class System
    {
        using ParticleClass = Particle<ParticleType>;
        using GraphClass = Graph<GraphType, ParticleClass, IsUnique>;

    public:

        class QEAIteratorKey
        {
        private:

            QEAIteratorKey() { }

            template<
                template <class, bool> class QGraphType,
                class QParticleType,        
                bool QIsUnique,
                SolutionEncoding QEncoding,
                AngleCalculationOperator QAngleCalculation,
                bool QGlobalRotation,
                bool QIterationRotation,
                bool QLocalRotation>
            friend class QEAIterator;
        };

        System(QEAIteratorKey, int stateCount, int stringLength, double limitAngle,
            InitializationType initialization, const std::vector<std::vector<std::vector<double>>>& heuristic,
            double heuristicInfluence)
            : stateCount_(stateCount),
              stringLength_(stringLength),
              limitAngle_(limitAngle),
              initialization_(initialization),
              heuristic_(heuristic),
              heuristicInfluence_(heuristicInfluence),
              bestCost_(DBL_MAX),
              rnd_(0, 1)
        {
            createGraph();
        }
        
        void measure(QEAIteratorKey, std::span<int> solutionString)
        {
            graph_->obtainString(typename GraphClass::SolverKey{}, solutionString);
        }

        void addRotation(QEAIteratorKey, double angle, std::span<const int> directions)
        {
            graph_->updateGraph(typename GraphClass::SolverKey{}, angle, directions);
        }

    private:

        UniformRandomDoubleGenerator rnd_;
        std::unique_ptr<Graph<GraphType, Particle<ParticleType>, IsUnique>> graph_;
        std::vector<std::unique_ptr<Particle<ParticleType>>> particles_;
        std::vector<std::vector<std::vector<double>>> heuristic_;
        double limitAngle_;
        double bestCost_;  
        double heuristicInfluence_;      
        int stateCount_;
        int stringLength_;
        int particleCount_;
        InitializationType initialization_;

        void createParticle(int ind1, int ind2)
        {
            if constexpr (std::is_same_v<ParticleType, BinaryParticle>)
            {
                if(initialization_ == InitializationType::Heuristic)
                {
                    particles_.emplace_back(std::make_unique<BinaryParticle>(typename ParticleClass::SystemKey{},
                        limitAngle_, heuristic_[ind1][ind2], heuristicInfluence_));
                }
                else
                {
                    particles_.emplace_back(std::make_unique<BinaryParticle>(typename ParticleClass::SystemKey{},
                        limitAngle_));
                }
            }
            else if constexpr (std::is_same_v<ParticleType, MultiLevelParticle>)
            {
                if(initialization_ == InitializationType::Heuristic)
                {
                    particles_.emplace_back(std::make_unique<MultiLevelParticle>(typename ParticleClass::SystemKey{},
                        stateCount_, limitAngle_, heuristic_[ind1][ind2], heuristicInfluence_));
                }
                else
                {
                    particles_.emplace_back(std::make_unique<MultiLevelParticle>(typename ParticleClass::SystemKey{},
                        stateCount_, limitAngle_));
                }
            }            
        }

        void createGraph()
        {
            if constexpr (std::is_same_v<GraphClass, Graph<MultiStageGraph, ParticleClass, IsUnique>>)
            {
                graph_= std::make_unique<MultiStageGraph<ParticleClass, IsUnique>>(typename GraphClass::SolverKey{},
                    stringLength_, stateCount_);
                particleCount_ = graph_->getVertexCount(typename GraphClass::SolverKey{});
                particles_.reserve(particleCount_);

                createParticle(0, 0);
                graph_->insertVertex(typename GraphClass::SolverKey{}, 0, 0, particles_.back().get());

                for(int i = 1; i < stringLength_; i++)
                {
                    for(int j = 0; j < stateCount_; j++)
                    {
                        createParticle(i, j);
                        graph_->insertVertex(typename GraphClass::SolverKey{}, i, j, particles_.back().get());
                    }
                }
            }
            else if constexpr (std::is_same_v<GraphClass, Graph<SingleStageGraph, ParticleClass, IsUnique>>)
            {
                graph_= std::make_unique<SingleStageGraph<ParticleClass, IsUnique>>(typename GraphClass::SolverKey{},
                    stringLength_, stateCount_);
                particleCount_ = graph_->getVertexCount(typename GraphClass::SolverKey{});
                particles_.reserve(particleCount_);

                for(int i = 0; i < stateCount_ + 1; i++)
                {
                    createParticle(0, i);
                    graph_->insertVertex(typename GraphClass::SolverKey{}, 0, i, particles_.back().get());
                }
            }
            else if constexpr (std::is_same_v<GraphClass, Graph<SequentialGraph, ParticleClass, IsUnique>>)
            {
                graph_= std::make_unique<SequentialGraph<ParticleClass, IsUnique>>(typename GraphClass::SolverKey{},
                    stringLength_, stateCount_);
                particleCount_ = graph_->getVertexCount(typename GraphClass::SolverKey{});
                particles_.reserve(particleCount_);

                for(int i = 0; i < stringLength_; i++)
                {
                    createParticle(0, i);
                    graph_->insertVertex(typename GraphClass::SolverKey{}, 0, i, particles_.back().get());
                }
            }
        }
    };
}