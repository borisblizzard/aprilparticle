/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "Particle.h"
#include "Rotator.h"

namespace aprilparticle
{
	Rotator::Rotator(float rotationSpeed)
	{
		this->rotationSpeed = rotationSpeed;
	}

	Rotator::~Rotator()
	{
	}
		
	void Rotator::update(Particle* particle, float k)
	{
		particle->angle += this->rotationSpeed * k;
	}

}

