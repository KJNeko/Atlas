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

template <>
QString engineNameT< ENGINES_BEGIN >()
{
	return "Unknown";
}

template <>
QString engineNameT< ENGINES_END >()
{
	return "Unknown";
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

template < Engine engine_t >
QString getEngineNameT( const Engine engine )
{
	if constexpr ( engine_t == ENGINES_END )
		return engineNameT< ENGINES_END >();
	else if constexpr ( engine_t == ENGINES_BEGIN )
		return engineNameT< static_cast< Engine >( ENGINES_BEGIN + 1 ) >();
	else
	{
		if ( engine == engine_t )
			return engineNameT< engine_t >();
		else
			return getEngineNameT< static_cast< Engine >( engine_t + 1 ) >( engine );
	}
}

QString engineName( const Engine engine )
{
	return getEngineNameT< ENGINES_BEGIN >( engine );
}
