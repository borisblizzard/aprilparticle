/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Color.h>
#include <hltypes/hltypesUtil.h>

#include "AffectorColorChanger.h"
#include "aprilparticleUtil.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		hmap<hstr, PropertyDescription> ColorChanger::_propertyDescriptions;

		ColorChanger::ColorChanger(chstr name) :
			Affector(name),
			_ratio(0.0f)
		{
			this->type = "ColorChanger";
			this->startColor = april::Color::White;
			this->endColor = april::Color::Blank;
		}
		
		ColorChanger::ColorChanger(const april::Color& endColor, chstr name) :
			Affector(name),
			_ratio(0.0f)
		{
			this->type = "ColorChanger";
			this->startColor = april::Color::White;
			this->endColor = endColor;
		}
		
		ColorChanger::ColorChanger(const april::Color& startColor, const april::Color& endColor, chstr name) :
			Affector(name),
			_ratio(0.0f)
		{
			this->type = "ColorChanger";
			this->startColor = startColor;
			this->endColor = endColor;
		}

		ColorChanger::ColorChanger(const ColorChanger& other) :
			Affector(other),
			_ratio(0.0f)
		{
			this->startColor = other.startColor;
			this->endColor = other.endColor;
		}

		Affector* ColorChanger::createInstance(chstr name)
		{
			return new ColorChanger(name);
		}

		hmap<hstr, PropertyDescription>& ColorChanger::getPropertyDescriptions() const
		{
			if (ColorChanger::_propertyDescriptions.size() == 0)
			{
				ColorChanger::_propertyDescriptions = Affector::getPropertyDescriptions();
				ColorChanger::_propertyDescriptions["start_color"] = PropertyDescription("start_color", PropertyDescription::Type::Color, april::Color::White.hex());
				ColorChanger::_propertyDescriptions["end_color"] = PropertyDescription("end_color", PropertyDescription::Type::Color, april::Color::Black.hex());
			}
			return ColorChanger::_propertyDescriptions;
		}

		void ColorChanger::setStartSymbolicColor(chstr value)
		{
			this->setStartColor(aprilparticle::_makeColor(value));
		}

		void ColorChanger::setEndSymbolicColor(chstr value)
		{
			this->setEndColor(aprilparticle::_makeColor(value));
		}

		hstr ColorChanger::getProperty(chstr name)
		{
			if (name == "start_color")	return this->getStartColor().hex();
			if (name == "end_color")	return this->getEndColor().hex();
			return Affector::getProperty(name);
		}

		bool ColorChanger::setProperty(chstr name, chstr value)
		{
			if		(name == "start_color")	this->setStartSymbolicColor(value);
			else if	(name == "end_color")	this->setEndSymbolicColor(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void ColorChanger::update(Particle* particle, float timeDelta, gvec3f& movement)
		{
			this->_ratio = particle->getLifeProgressRatio();
			particle->color = this->startColor * (1.0f - this->_ratio) + this->endColor * this->_ratio;
		}

	}

}

