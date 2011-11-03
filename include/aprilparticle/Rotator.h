/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a rotation affector.

#ifndef APRILPARTICLE_ROTATOR_H
#define APRILPARTICLE_ROTATOR_H

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	class aprilparticleExport Rotator : public Affector
	{
	public:
		Rotator(float rotationSpeed = 1.0f);
		~Rotator();

		float getRotationSpeed() { return this->rotationSpeed; }
		void setRotationSpeed(float value) { this->rotationSpeed = value; }
				
		void update(Particle* particle, float k);

	protected:
		float rotationSpeed;

	};

}

#endif
