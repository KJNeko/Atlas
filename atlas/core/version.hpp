//
// Created by kj16609 on 5/9/23.
//

#ifndef ATLASGAMEMANAGER_VERSION_HPP
#define ATLASGAMEMANAGER_VERSION_HPP

#ifdef NDEBUG
#ifndef ATLAS_MAJOR
#define ATLAS_MAJOR 0
#endif

#ifndef ATLAS_MINOR
#define ATLAS_MINOR 0
#endif

#ifndef ATLAS_PATCH
#define ATLAS_PATCH 0
#endif

#define ATLAS_VERSION ATLAS_MAJOR * 10000 + ATLAS_MINOR * 100 + ATLAS_PATCH

#if ATLAS_VERSION == 0
#error "Atlas tried to compile for version 0.0.0. Please define the major minor and patch via cmake"
#endif

#define STR( str ) #str
#define STRING( str ) STR( str )

#define ATLAS_VERSION_STR "v" STRING( ATLAS_MAJOR ) "." STRING( ATLAS_MINOR ) "." STRING( ATLAS_PATCH )
#else
#define ATLAS_VERSION_STR "Debug"
#endif

#endif //ATLASGAMEMANAGER_VERSION_HPP
