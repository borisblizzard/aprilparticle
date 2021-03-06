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
/// Represents an independent GUI object that controls a particle emitter.

#ifndef APRILUIPARTICLE_PARTICLE_EMITTER_H
#define APRILUIPARTICLE_PARTICLE_EMITTER_H

#include <aprilui/Object.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"

namespace aprilparticle
{
	class Emitter;
	class System;
}

namespace apriluiparticle
{
	class Space;

	class apriluiparticleExport Emitter : public aprilui::Object
	{
		APRILUI_CLONEABLE(Emitter);
	public:
		friend class Space;

		Emitter(chstr name);
		~Emitter();
		inline hstr getClassName() const override { return "apriluiparticle.Emitter"; }
		static aprilui::Object* createInstance(chstr name);

		hmap<hstr, aprilui::PropertyDescription>& getPropertyDescriptions() const override;

		HL_DEFINE_GET(Space*, spaceObject, SpaceObject);
		HL_DEFINE_GET(aprilparticle::Emitter*, emitter, Emitter);
		HL_DEFINE_GETSET(hstr, spaceObjectName, SpaceObjectName);
		HL_DEFINE_GETSET(hstr, emitterName, EmitterName);

		void resetEmitter();

		void notifyEvent(chstr type, aprilui::EventArgs* args) override;
		
	protected:
		Space* spaceObject;
		aprilparticle::Emitter* emitter;
		hstr spaceObjectName;
		hstr emitterName;
		gvec2f initialPosition;
		float initialAngle;
		gvec3f emitterPosition;
		gvec3f emitterMinDirection;
		gvec3f emitterMaxDirection;
		gvec2f emitterMinSize;
		gvec2f emitterMaxSize;

		hmap<hstr, aprilui::PropertyDescription::Accessor*>& _getGetters() const override;
		hmap<hstr, aprilui::PropertyDescription::Accessor*>& _getSetters() const override;

		void _update(float timeDelta) override;
		void _draw() override;

		void _updateEmitterData();
		void _updateBindings();
		void _tryFindSpaceObject();
		void _tryFindEmitter();
		void _unbind();

	private:
		static hmap<hstr, aprilui::PropertyDescription> _propertyDescriptions;
		static hmap<hstr, aprilui::PropertyDescription::Accessor*> _getters;
		static hmap<hstr, aprilui::PropertyDescription::Accessor*> _setters;

	};

}
#endif