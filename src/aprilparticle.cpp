/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <hltypes/hexception.h>
#include <hltypes/hlog.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "Affectors.h"
#include "aprilparticle.h"
#include "System.h"
#include "Texture.h"

#define REGISTER_AFFECTOR_TYPE(name) aprilparticle::registerAffectorFactory(#name, &Affectors::name::createInstance)

namespace aprilparticle
{
	hstr logTag = "aprilparticle";

	static hversion version(3, 0, 0);

	static bool useCache = true;
	static bool throwExceptionOnTextureFailure = true;
	static hmap<hstr, Affector* (*)(chstr)> gAffectorFactories;
	static hmap<hstr, aprilparticle::Texture*> gTextureCache;
	static hmap<hstr, System*> gSystemCache;

	extern void initForceField();
	void init()
	{
		hlog::write(logTag, "Initializing AprilParticle: " + version.toString());
		initForceField();
		useCache = true;
		REGISTER_AFFECTOR_TYPE(Attractor);
		REGISTER_AFFECTOR_TYPE(CallbackAffector);
		REGISTER_AFFECTOR_TYPE(ColorChanger);
		REGISTER_AFFECTOR_TYPE(ColorChangerTimed);
		REGISTER_AFFECTOR_TYPE(ForceField);
		REGISTER_AFFECTOR_TYPE(LinearForce);
		REGISTER_AFFECTOR_TYPE(Resizer);
		REGISTER_AFFECTOR_TYPE(ResizerTimed);
		REGISTER_AFFECTOR_TYPE(Revolutor);
		REGISTER_AFFECTOR_TYPE(Rotator);
		REGISTER_AFFECTOR_TYPE(Scaler);
		REGISTER_AFFECTOR_TYPE(ScalerTimed);
	}
	
	void destroy()
	{
		hlog::write(logTag, "Destroying AprilParticle.");
		gAffectorFactories.clear();
		foreach_m (aprilparticle::System*, it, gSystemCache)
		{
			delete it->second;
		}
		gSystemCache.clear();
		foreach_m (aprilparticle::Texture*, it, gTextureCache)
		{
			delete it->second;
		}
		gTextureCache.clear();
	}

	bool isUseCache()
	{
		return useCache;
	}

	void setUseCache(const bool& value)
	{
		useCache = value;
	}

	bool isThrowExceptionOnTextureFailure()
	{
		return throwExceptionOnTextureFailure;
	}

	void setThrowExceptionOnTextureFailure(const bool& value)
	{
		throwExceptionOnTextureFailure = value;
	}

	void registerAffectorFactory(chstr typeName, Affector* (*factory)(chstr))
	{
		if (gAffectorFactories.hasKey(typeName))
		{
			throw Exception("Affector Factory '" + typeName + "' already exists!");
		}
		gAffectorFactories[typeName] = factory;
		// create all important property meta data here already to avoid issues with multi-threaded first-time setup
		Affector* temp = (*factory)("temp");
		temp->getPropertyDescriptions();
		temp->getProperty("name");
		temp->setProperty("name", "");
		delete temp;
	}

	Affector* createAffector(chstr typeName, chstr name)
	{
		if (gAffectorFactories.hasKey(typeName))
		{
			return (*gAffectorFactories[typeName])(name);
		}
		return NULL;
	}

	aprilparticle::Texture* loadTexture(chstr filename, bool cached)
	{
		aprilparticle::Texture* texture = NULL;
		if (gTextureCache.hasKey(filename))
		{
			texture = gTextureCache[filename];
		}
		else
		{
			bool cachedTexture = (cached || useCache);
			april::Texture* aprilTexture = april::rendersys->createTextureFromResource(filename, april::Texture::Type::Immutable, cachedTexture ? april::Texture::LoadMode::Async : april::Texture::LoadMode::OnDemand);
			if (aprilTexture != NULL)
			{
				texture = new aprilparticle::Texture(aprilTexture, filename, cachedTexture, cached);
				if (cachedTexture)
				{
					gTextureCache[filename] = texture;
				}
			}
			else if (throwExceptionOnTextureFailure)
			{
				throw FileCouldNotOpenException(filename);
			}
		}
		return texture;
	}

	void clearCache()
	{
		hlog::write(logTag, "clearing cache");
		foreach_m (aprilparticle::Texture*, it, gTextureCache)
		{
			it->second->getTexture()->unload();
		}
		foreach_m (aprilparticle::System*, it, gSystemCache)
		{
			delete it->second;
		}
		gSystemCache.clear();
	}

	System* loadSystem(chstr filename, chstr name)
	{
		hstr key = name;
		if (key == "")
		{
			key = filename;
		}
		aprilparticle::System* system = gSystemCache.tryGet(key, NULL);
		if (system != NULL)
		{
			return new aprilparticle::System(*system);
		}
		system = new aprilparticle::System(filename, name);
		system->load();
		if (useCache)
		{
			gSystemCache[key] = system;
			system = new aprilparticle::System(*system);
		}
		return system;
	}

}
