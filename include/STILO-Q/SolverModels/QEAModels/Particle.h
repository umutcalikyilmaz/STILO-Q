#pragma once
#include <math.h>
#include "STILO-Q/Shared/List.h"
#include "STILO-Q/Shared/Random.h"
#include "STILO-Q/SolverModels/GraphModels/Vertex.h"

namespace QEVO
{
    template <class ParticleType>
    class Particle : public Vertex<Particle<ParticleType>>
    {
	public:

		class VertexKey
		{
		private:
			
			VertexKey() { }

			friend class Vertex<Particle<ParticleType>>;
		};

		class ParticleKey
		{
		private:

			ParticleKey() { }

			friend class Particle<ParticleType>;
		};

		class SystemKey
		{
		private:
			
			SystemKey() { }

			template<
				template <class, bool> class SGraphType,
				class SParticleType,
				bool SIsUnique>
			friend class System;
 		};
		/*
		void initialize()
		{
			static_cast<ParticleType*>(this)->initialize();
		}
		*/
		/*
		void initializeRandom()
		{
			static_cast<ParticleType*>(this)->initializeRandom();
		}		
		*/
		
		int measureExplicit(VertexKey)
		{
			return static_cast<ParticleType*>(this)->measureExplicit(typename ParticleType::ParticleKey{});
		}

		int measureExplicit(VertexKey, const std::vector<int>& permitted, int pnum)
		{
			return static_cast<ParticleType*>(this)->measureExplicit(typename ParticleType::ParticleKey{}, permitted, pnum);
		}

		void addUpdateExplicit(VertexKey, double angle, int direction)
		{
			static_cast<ParticleType*>(this)->addUpdateExplicit(typename ParticleType::ParticleKey{}, angle, direction);
		}

		virtual ~Particle() = default;

    protected:
		
		UniformRandomDoubleGenerator rnd;
		double limitAngle;
		double upperLimitAngle;
		bool needUpdate;

		Particle(int stateCount, double limitAngle)
			: Vertex<Particle<ParticleType>>(stateCount),
			  limitAngle(limitAngle),
			  upperLimitAngle(M_PI_2 - limitAngle),
			  rnd(0, 1),
			  needUpdate(false) { }
    };
}