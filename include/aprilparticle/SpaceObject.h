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
/// Represents an object in space.

#ifndef APRILPARTICLE_SPACE_OBJECT_H
#define APRILPARTICLE_SPACE_OBJECT_H

#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "ActiveObject.h"
#include "aprilparticleExport.h"

namespace aprilparticle
{
	class aprilparticleExport SpaceObject : public ActiveObject
	{
	public:
		SpaceObject(chstr name = "");
		SpaceObject(const SpaceObject& other);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

		HL_DEFINE_GETSET(gvec3f, position, Position);
		inline void setPosition(float x, float y, float z) { this->position.set(x, y, z); }

		hstr getProperty(chstr name) override;
		bool setProperty(chstr name, chstr value) override;

	protected:
		gvec3f position;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;

	};
}

#endif
