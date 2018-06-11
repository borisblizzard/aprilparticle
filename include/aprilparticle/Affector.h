/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a generic affector class.

#ifndef APRILPARTICLE_AFFECTOR_H
#define APRILPARTICLE_AFFECTOR_H

#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "aprilparticleExport.h"
#include "PropertyDescription.h"

#define APRILPARTICLE_CLONEABLE(type) \
public: \
	virtual type* clone() const { return new type(*this); } \
protected: \
	type(const type& other);

#define APRILPARTICLE_CLONEABLE_ABSTRACT(type) \
public: \
	virtual type* clone() const = 0; \
protected: \
	type(const type& other);

namespace aprilparticle
{
	class Particle;
	class Space;
	class System;

	class aprilparticleExport Affector
	{
		APRILPARTICLE_CLONEABLE_ABSTRACT(Affector);
	public:
		friend class System;
		friend class Space;

		Affector(chstr name = "");
		virtual ~Affector();

		virtual hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_IS(autogeneratedName, AutogeneratedName);
		HL_DEFINE_GET(hstr, name, Name);
		void setName(chstr value);
		HL_DEFINE_GET(hstr, type, Type);
		HL_DEFINE_GETSET(float, minRandomness, MinRandomness);
		HL_DEFINE_GETSET(float, maxRandomness, MaxRandomness);
		HL_DEFINE_GETSET(gvec3f, chaoticity, Chaoticity);
		void setRandomness(float value);
		void setRandomness(chstr value);

		void setRandomnessRange(float min, float max);

		virtual inline void update(Particle* particle, float timeDelta, gvec3f& movement) { }
		virtual inline void draw() { }

		virtual hstr getProperty(chstr name);
		virtual bool setProperty(chstr name, chstr value);

	protected:
		bool autogeneratedName;
		hstr name;
		hstr type;
		float minRandomness;
		float maxRandomness;
		gvec3f chaoticity;
		Space* _space;

		inline void _setSpace(Space* value) { this->_space = value; }

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;

	};
}

#endif
