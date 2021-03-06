/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a force field affector.

#ifndef APRILPARTICLE_AFFECTORS_FORCE_FIELD_H
#define APRILPARTICLE_AFFECTORS_FORCE_FIELD_H

#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "AffectorSpace.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport ForceField : public Space
		{
			APRILPARTICLE_CLONEABLE(ForceField);
		public:
			ForceField(chstr name = "");
			ForceField(cgvec3f position, float radius, cgvec3f direction, chstr name = "");
			static Affector* createInstance(chstr name = "");

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

			HL_DEFINE_GETSET(gvec3f, direction, Direction);
			inline void setDirection(float x, float y, float z) { this->direction.set(x, y, z); }
				
			hstr getProperty(chstr name) override;
			bool setProperty(chstr name, chstr value) override;

			void update(Particle* article, float timeDelta, gvec3f& movement) override;
			void draw() override;

		protected:
			gvec3f direction;

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

			float _factor;
			float _squaredLength;
			
		};

	}
}
#endif
