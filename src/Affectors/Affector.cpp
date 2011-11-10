/// @file
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hstring.h>

#include "Affector.h"
#include "Util.h"

namespace aprilparticle
{
	Affector::Affector(chstr name)
	{
		this->name = (name == "" ? generateName("Affector") : name);
	}

	Affector::~Affector()
	{
	}

	hstr Affector::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "name")	return this->getName();
		if (property_exists != NULL)
		{
			*property_exists = false;
		}
		return "";
	}
	
	bool Affector::setProperty(chstr name, chstr value)
	{
		if (name == "name")	this->setName(value);
		else return false;
		return true;
	}

}
