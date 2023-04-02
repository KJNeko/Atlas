//
// Created by kj16609 on 4/1/23.
//

#include "engineDetection.hpp"

template <>
bool isEngine< ENGINES_BEGIN >( [[maybe_unused]] const std::filesystem::path& path )
{
	return false;
}

template <>
bool isEngine< ENGINES_END >( [[maybe_unused]] const std::filesystem::path& path )
{
	return true;
}

template < Engine engine >
Engine findEngine( const std::filesystem::path& path )
{
	if constexpr ( engine == ENGINES_END )
		return ENGINES_END;
	else
	{
		if ( isEngine< engine >( path ) )
			return engine;
		else
			return findEngine< static_cast< Engine >( engine + 1 ) >( path );
	}
}

Engine determineEngine( const std::filesystem::path path )
{
	return findEngine< ENGINES_BEGIN >( path );
}
