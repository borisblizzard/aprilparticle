/// @file
/// @author  Boris Mikic
/// @version 1.6
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents an active object.

#ifndef APRILPARTICLE_ACTIVE_OBJECT_H
#define APRILPARTICLE_ACTIVE_OBJECT_H

#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Affector;

	class aprilparticleExport ActiveObject
	{
	public:
		ActiveObject(chstr name = "");
		ActiveObject(const ActiveObject& other);
		virtual ~ActiveObject();

		HL_DEFINE_GETSET(hstr, name, Name);
		HL_DEFINE_GETSET(gvec3, position, Position);
		void setPosition(float x, float y, float z) { this->position.set(x, y, z); }
		HL_DEFINE_ISSET(visible, Visible);
		HL_DEFINE_ISSET(enabled, Enabled);
		HL_DEFINE_GET(harray<Affector*>, affectors, Affectors);

		virtual void addAffector(Affector* affector);
		virtual void removeAffector(Affector* affector);

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

		Affector* getAffector(chstr name);
		template <class T>
		T* getAffector(chstr name)
		{
			return dynamic_cast<T*>(this->getAffector(name));
		}

	protected:
		hstr name;
		gvec3 position;
		bool visible;
		bool enabled;
		harray<Affector*> affectors;

		hmap<Affector*, hmap<hstr, hstr> > _affectorProperties;

	};
}

#endif
