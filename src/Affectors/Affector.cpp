/// @file
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "Affector.h"
#include "aprilparticle.h"
#include "aprilparticleUtil.h"

namespace aprilparticle
{
	Affector::Affector(chstr name)
	{
		this->name = (name == "" ? generateName("Affector") : name);
		this->minRandomness = 1.0f;
		this->maxRandomness = 1.0f;
		this->chaoticity.set(0.0f, 0.0f, 0.0f);
		this->space = NULL;
	}

	Affector::~Affector()
	{
	}

	void Affector::setRandomness(float value)
	{
		this->minRandomness = value;
		this->maxRandomness = value;
	}

	void Affector::setRandomness(chstr value)
	{
		harray<hstr> data = value.split(aprilparticle::SeparatorRange);
		this->setRandomnessRange(data.first(), data.last());
	}

	void Affector::setRandomnessRange(float min, float max)
	{
		this->minRandomness = min;
		this->maxRandomness = max;
	}

	hstr Affector::getProperty(chstr name)
	{
		if (name == "name")			return this->getName();
		if (name == "randomness")	return GET_RANGE(Randomness, hstr);
		//if (name == "chaoticity")	return gvec3_to_hstr(this->getChaoticity());
		hlog::warnf(aprilparticle::logTag, "Affector property '%s' does not exist in '%s'!", name.c_str(), this->name.c_str());
		return "";
	}
	
	bool Affector::setProperty(chstr name, chstr value)
	{
		if		(name == "name")		this->setName(value);
		else if	(name == "randomness")	this->setRandomness(value);
		//else if	(name == "chaoticity")	this->setChaoticity(hstr_to_gvec3(value));
		else
		{
			hlog::warnf(aprilparticle::logTag, "Affector property '%s' does not exist in '%s'!", name.c_str(), this->name.c_str());
			return false;
		}
		return true;
	}

}
