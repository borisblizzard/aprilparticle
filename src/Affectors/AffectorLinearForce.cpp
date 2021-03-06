/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/aprilUtil.h>

#include "AffectorLinearForce.h"
#include "aprilparticleUtil.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		hmap<hstr, PropertyDescription> LinearForce::_propertyDescriptions;

		LinearForce::LinearForce(chstr name) :
			Affector(name)
		{
			this->type = "LinearForce";
			this->direction.set(0.0f, -1.0f, 0.0f);
		}

		LinearForce::LinearForce(cgvec3f direction, chstr name) :
			Affector(name)
		{
			this->type = "LinearForce";
			this->direction = direction;
		}

		LinearForce::LinearForce(const LinearForce& other) :
			Affector(other)
		{
			this->direction = other.direction;
		}

		Affector* LinearForce::createInstance(chstr name)
		{
			return new LinearForce(name);
		}

		hmap<hstr, PropertyDescription>& LinearForce::getPropertyDescriptions() const
		{
			if (LinearForce::_propertyDescriptions.size() == 0)
			{
				LinearForce::_propertyDescriptions = Affector::getPropertyDescriptions();
				LinearForce::_propertyDescriptions["direction"] = PropertyDescription("direction", PropertyDescription::Type::Gvec3f, april::gvec3ToHstr<float>(gvec3f(0.0f, -1.0f, 0.0f)));
			}
			return LinearForce::_propertyDescriptions;
		}

		hstr LinearForce::getProperty(chstr name)
		{
			if (name == "direction")	return april::gvec3ToHstr<float>(this->getDirection());
			return Affector::getProperty(name);
		}

		bool LinearForce::setProperty(chstr name, chstr value)
		{
			if		(name == "direction")	this->setDirection(april::hstrToGvec3<float>(value));
			else return Affector::setProperty(name, value);
			return true;
		}

		void LinearForce::update(Particle* particle, float timeDelta, gvec3f& movement)
		{
			particle->direction += this->direction * (RAND_RANGE(Randomness) * timeDelta);
		}

	}

}

