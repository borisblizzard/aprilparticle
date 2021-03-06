/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Color.h>
#include <aprilparticle/aprilparticle.h>
#include <aprilparticle/Emitter.h>
#include <aprilparticle/Space.h>
#include <aprilparticle/System.h>
#include <aprilui/aprilui.h>
#include <aprilui/Dataset.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "apriluiparticleUtil.h"
#include "Emitter.h"
#include "Space.h"
#include "System.h"

namespace apriluiparticle
{
	hmap<hstr, aprilui::PropertyDescription> Space::_propertyDescriptions;
	hmap<hstr, aprilui::PropertyDescription::Accessor*> Space::_getters;
	hmap<hstr, aprilui::PropertyDescription::Accessor*> Space::_setters;

	Space::Space(chstr name) :
		aprilui::Object(name)
	{
		this->systemObject = NULL;
		this->space = NULL;
		this->debugColor.set(april::Color::LightMagenta, 32);
	}
	
	Space::Space(const Space& other) :
		aprilui::Object(other)
	{
		this->systemObject = NULL;
		this->space = NULL;
		this->systemObjectName = other.systemObjectName;
		this->spaceName = other.spaceName;
	}

	Space::~Space()
	{
		if (this->systemObject != NULL)
		{
			this->systemObject->_unregisterSpaceObject(this);
		}
		foreach (apriluiparticle::Emitter*, it, this->emitterObjects)
		{
			(*it)->_unbind();
			(*it)->spaceObject = NULL;
		}
	}

	aprilui::Object* Space::createInstance(chstr name)
	{
		return new Space(name);
	}

	hmap<hstr, aprilui::PropertyDescription>& Space::getPropertyDescriptions() const
	{
		if (Space::_propertyDescriptions.size() == 0)
		{
			Space::_propertyDescriptions = aprilui::Object::getPropertyDescriptions();
			Space::_propertyDescriptions["system_object"] = aprilui::PropertyDescription("system_object", aprilui::PropertyDescription::Type::String);
			Space::_propertyDescriptions["space"] = aprilui::PropertyDescription("space", aprilui::PropertyDescription::Type::String);
		}
		return Space::_propertyDescriptions;
	}


	hmap<hstr, aprilui::PropertyDescription::Accessor*>& Space::_getGetters() const
	{
		if (Space::_getters.size() == 0)
		{
			Space::_getters = aprilui::Object::_getGetters();
			Space::_getters["system_object"] = new aprilui::PropertyDescription::Get<Space, hstr>(&Space::getSystemObjectName);
			Space::_getters["space"] = new aprilui::PropertyDescription::Get<Space, hstr>(&Space::getSpaceName);
		}
		return Space::_getters;
	}

	hmap<hstr, aprilui::PropertyDescription::Accessor*>& Space::_getSetters() const
	{
		if (Space::_setters.size() == 0)
		{
			Space::_setters = aprilui::Object::_getSetters();
			Space::_setters["system_object"] = new aprilui::PropertyDescription::Set<Space, hstr>(&Space::setSystemObjectName);
			Space::_setters["space"] = new aprilui::PropertyDescription::Set<Space, hstr>(&Space::setSpaceName);
		}
		return Space::_setters;
	}

	void Space::_update(float timeDelta)
	{
		this->_updateBindings();
		foreach (apriluiparticle::Emitter*, it, this->emitterObjects)
		{
			(*it)->_updateBindings();
		}
		if (this->space != NULL)
		{
			this->space->setEnabled(this->isDerivedEnabled());
			this->space->update(timeDelta);
		}
		aprilui::Object::_update(timeDelta);
	}

	void Space::_draw()
	{
		if (this->space != NULL)
		{
			this->space->setVisible(this->isVisible());
			this->space->draw(this->_makeDrawRect().getCenter(), this->_makeDrawColor());
		}
		aprilui::Object::_draw();
	}

	void Space::_updateBindings()
	{
		this->_tryFindSystemObject();
		this->_tryFindSpace();
	}

	void Space::_tryFindSystemObject()
	{
		if (this->dataset == NULL)
		{
			this->systemObject = NULL;
			return;
		}
		if (this->systemObject != NULL && this->systemObject->getName() == this->systemObjectName)
		{
			return;
		}
		this->systemObject = NULL;
		this->space = NULL;
		if (this->systemObjectName == "")
		{
			return;
		}
		this->systemObject = dynamic_cast<System*>(this->dataset->tryGetObject(this->systemObjectName));
		if (this->systemObject != NULL)
		{
			this->systemObject->_registerSpaceObject(this);
		}
		else
		{
			hlog::warnf(logTag, "Space '%s': referenced object '%s' not a subclass of System!", this->name.cStr(), this->systemObjectName.cStr());
			this->systemObjectName = "";
			this->spaceName = "";
		}
	}

	void Space::_tryFindSpace()
	{
		if (this->systemObject == NULL)
		{
			this->space = NULL;
			return;
		}
		if (this->space != NULL && this->space->getName() == this->spaceName)
		{
			return;
		}
		this->space = NULL;
		if (this->spaceName == "")
		{
			return;
		}
		aprilparticle::System* system = this->systemObject->getSystem();
		if (system == NULL)
		{
			return;
		}
		this->space = system->getSpace(this->spaceName);
		if (this->space == NULL)
		{
			hlog::warnf(logTag, "Space '%s': cannot find space '%s' in System '%s'!", this->name.cStr(), this->spaceName.cStr(), this->systemObject->getName().cStr());
			this->spaceName = "";
		}
	}

	void Space::notifyEvent(chstr type, aprilui::EventArgs* args)
	{	
		if (type == aprilui::Event::SizeChanged)
		{
			this->_resize();
		}
		aprilui::Object::notifyEvent(type, args);
	}

	void Space::_registerEmitterObject(Emitter* emitter)
	{
		this->emitterObjects += emitter;
	}

	void Space::_unregisterEmitterObject(Emitter* emitter)
	{
		this->emitterObjects -= emitter;
	}

	void Space::_unbind()
	{
		this->space = NULL;
		foreach (apriluiparticle::Emitter*, it, this->emitterObjects)
		{
			(*it)->_unbind();
		}
	}
	
	void Space::_resize()
	{
		if (this->space != NULL)
		{
			apriluiparticle::resizeEmitters(this->getSize(), this->space->getEmitters());
		}
	}

}
