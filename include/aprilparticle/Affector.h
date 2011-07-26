#ifndef AFFECTOR_H
#define AFFECTOR_H

#include "aprilparticleExport.h"

namespace april
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Affector
		{
		public:
			Affector() { }
			virtual ~Affector() { }
				
			virtual void update(Particle* particle, float k) { }
			virtual void draw() { }

		};

	}

}

#endif
