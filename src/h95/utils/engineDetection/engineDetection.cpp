//
// Created by kj16609 on 4/1/23.
//

#include "engineDetection.hpp"

template <>
bool isEngine< ENGINES_BEGIN >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
{
	return false;
}

template <>
bool isEngine< ENGINES_END >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
Engine findEngine( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{
	if constexpr ( engine == ENGINES_END )
		return ENGINES_END;
	else
	{
		if ( isEngine< engine >( path, executable_path ) )
			return engine;
		else
			return findEngine< static_cast< Engine >( engine + 1 ) >( path, executable_path );
	}
}

Engine determineEngine( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{
	return findEngine< ENGINES_BEGIN >( path, executable_path );
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

//Define all specializations of isEngine and engineNameT here

template <>
bool isEngine< RenPy >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{
	return std::filesystem::exists( path / "renpy" );
}

template <>
QString engineNameT< RenPy >()
{
	return "Ren'Py";
}

template <>
bool isEngine< Unity >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{
	return std::filesystem::exists( path / "Data" / "Managed" / "Assembly-CSharp.dll" );
}

template <>
QString engineNameT< Unity >()
{
	return "Unity";
}

template <>
bool isEngine< Unreal >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{}

template <>
QString engineNameT< Unreal >()
{
	return "Unreal";
}

template <>
bool isEngine< RPGM >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{}

template <>
QString engineNameT< RPGM >()
{
	return "RPG Maker";
}

template <>
bool isEngine< WolfRPG >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{}

template <>
QString engineNameT< WolfRPG >()
{
	return "Wolf RPG";
}

template <>
bool isEngine< HTML >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{
	return std::filesystem::exists( path / "index.html" );
}

template <>
QString engineNameT< HTML >()
{
	return "HTML";
}

template <>
bool isEngine< VisualNovelMaker >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{}

template <>
QString engineNameT< VisualNovelMaker >()
{
	return "Visual Novel Maker";
}

template <>
bool isEngine< TyanoBuilder >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{}

template <>
QString engineNameT< TyanoBuilder >()
{
	return "TyanoBuilder Visual Novel Software";
}

template <>
bool isEngine< Java >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{}

template <>
QString engineNameT< Java >()
{
	return "Java";
}

template <>
bool isEngine< Flash >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{
	return std::filesystem::exists( path / "index.html" );
}

template <>
QString engineNameT< Flash >()
{
	return "Flash";
}

template <>
bool isEngine< RAGS >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{}

template <>
QString engineNameT< RAGS >()
{
	return "RAGS";
}

template <>
bool isEngine< KiriKiri >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{}

template <>
QString engineNameT< KiriKiri >()
{
	return "KiriKiri";
}

template <>
bool isEngine< NScripter >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{}

template <>
QString engineNameT< NScripter >()
{
	return "NScripter";
}

template <>
bool isEngine< NVList >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{}

template <>
QString engineNameT< NVList >()
{
	return "NVList";
}

template <>
bool isEngine< Sukai2 >( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{}

template <>
QString engineNameT< Sukai2 >()
{
	return "Sukai2";
}
