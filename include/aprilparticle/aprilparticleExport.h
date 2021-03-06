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
/// Defines macros for DLL exports/imports.

#ifndef APRILPARTICLE_EXPORT_H
#define APRILPARTICLE_EXPORT_H

	#ifdef _LIB
		#define aprilparticleExport
		#define aprilparticleFnExport
	#else
		#ifdef _WIN32
			#ifdef APRILPARTICLE_EXPORTS
				#define aprilparticleExport __declspec(dllexport)
				#define aprilparticleFnExport __declspec(dllexport)
			#else
				#define aprilparticleExport __declspec(dllimport)
				#define aprilparticleFnExport __declspec(dllimport)
			#endif
		#else
			#define aprilparticleExport __attribute__ ((visibility("default")))
			#define aprilparticleFnExport __attribute__ ((visibility("default")))
		#endif
	#endif

#endif

