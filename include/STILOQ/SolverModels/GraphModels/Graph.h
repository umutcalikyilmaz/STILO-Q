#pragma once
#include "STILOQ/SolverModels/GraphModels/Vertex.h"
#include "STILOQ/Shared/Containers.h"
#include "STILOQ/Shared/List.h"
#include "STILOQ/Shared/Parameters.h"

namespace STILOQ
{
    template<
        template <class, bool> class GraphType,
        class VertexType,
        bool IsUnique>
    class Graph
    {
    public:

        class SolverKey
        {
        private:
            
            SolverKey() {}

            template<
                template <class, bool> class PGraphType,
                bool PIsUnique,
                PheromoneUpdateOperator PPheromoneUpdate,
                bool PLocalUpdate,
                bool PHeuristic>
            friend class Pheromone;

            template<
                template <class, bool> class SGraphType,
                class SParticleType,
                bool SIsUnique>
            friend class System;
        };

        class GraphKey
        {
        private:

            GraphKey() { }
            
            friend class Graph<GraphType, VertexType, IsUnique>;
        };

        void insertVertex(SolverKey, int ind1, int ind2, Vertex<VertexType>* vertex)
        {
            static_cast<GraphType<VertexType,IsUnique>*>(this)->insertVertexExplicit(GraphKey{}, ind1, ind2, vertex);
        }        

        int getVertexCount(SolverKey)
        {
            return vertexCount;
        }

        void obtainString(SolverKey, std::span<int> solutionString)
        {
            static_cast<GraphType<VertexType,IsUnique>*>(this)->obtainStringExplicit(GraphKey{}, solutionString);
        }

        void updateGraph(SolverKey, double value, std::span<const int> list)
        {
            static_cast<GraphType<VertexType,IsUnique>*>(this)->updateGraphExplicit(GraphKey{}, value, list);
        }

        virtual ~Graph() = default; 
        
    protected:
    
        std::vector<int> permitted;
        int stateCount;
        int stringLength;
        int vertexCount;

        Graph(int stringLength, int stateCount) 
            : stringLength(stringLength),
              stateCount(stateCount),
              permitted(stateCount) { }
    };    
}