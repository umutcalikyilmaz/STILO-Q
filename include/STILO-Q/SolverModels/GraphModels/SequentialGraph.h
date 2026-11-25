#pragma once
#include "STILO-Q/SolverModels/GraphModels/Graph.h"

namespace QEVO
{
    template<
        class VertexType,
        bool IsUnique> 
    class SequentialGraph final : public Graph<SequentialGraph, VertexType, IsUnique>
    {

        using VertexClass = Vertex<VertexType>;
        using BaseClass = Graph<SequentialGraph, VertexType, IsUnique>;

    public:

        SequentialGraph(BaseClass::SolverKey, int stringLength, int stateCount) 
            : Graph<SequentialGraph,VertexType, IsUnique>(stringLength, stateCount),
              vertices_(stringLength)
        {
            this->vertexCount = stringLength;            
        }

        void insertVertexExplicit(BaseClass::GraphKey, int ind1, int ind2, Vertex<VertexType>* vertex)
        {
            vertices_[ind2] = vertex;
        }

        void obtainStringExplicit(BaseClass::GraphKey, std::span<int> solutionString)
        {
            if constexpr (IsUnique)
            {
                int num = this->stateCount;
    
                for(int i = 0; i < this->stateCount; i++)
                {
                    this->permitted[i] = i;
                }

                for(int i = 0; i < this->stringLength - 1; i++)
                {			
                    int ind = vertices_[i]->measure(typename VertexClass::GraphKey{}, this->permitted, num);
                    solutionString[i] = this->permitted[ind];
                    deleteRandom(this->permitted, ind, num);
                    num--;
                }
    
                solutionString[this->stringLength - 1] = this->permitted[0];
            }
            else
            {
                for(int i = 0; i < this->stringLength; i++)
                {
                    solutionString[i] = vertices_[i]->measure(typename VertexClass::GraphKey{});
                }
            }
        }

        void updateGraphExplicit(BaseClass::GraphKey, double value, std::span<const int> list)
        {
            for(int i = 0; i < this->stringLength; i++)
            {
                vertices_[i]->addUpdate(typename VertexClass::GraphKey{}, value, list[i]);
            }
        }

    private:

        std::vector<Vertex<VertexType>*> vertices_;
    };
}