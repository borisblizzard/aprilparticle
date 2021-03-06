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
/// Represents a scaler with timed values.

#ifndef APRILPARTICLE_AFFECTORS_SCALER_TIMED_H
#define APRILPARTICLE_AFFECTORS_SCALER_TIMED_H

#include <april/Color.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport ScalerTimed : public Affector
		{
			APRILPARTICLE_CLONEABLE(ScalerTimed);
		public:
			ScalerTimed(chstr name = "");
			ScalerTimed(const hmap<float, float>& timings, chstr name = "");
			static Affector* createInstance(chstr name = "");

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

			HL_DEFINE_GET(harray<float>, times, Times);
			HL_DEFINE_GET(harray<float>, values, Values);
			void setTimings(const hmap<float, float>& value);
			void setTimings(chstr value);

			hstr getProperty(chstr name) override;
			bool setProperty(chstr name, chstr value) override;

			void addTiming(float time, float value);
			
			void update(Particle* emitter, float timeDelta, gvec3f& movement) override;
			
		protected:
			harray<float> times;
			harray<float> values;
			int size;

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

			float _ratio;
			int _i;

		};

	}
}
#endif
