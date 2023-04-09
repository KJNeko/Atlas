//
// Created by kj16609 on 4/1/23.
//

#include "engineDetection.hpp"

#include <iostream>

#include <QMimeDatabase>

#include <tracy/Tracy.hpp>

#include "../../system.hpp"
#include "h95/logging.hpp"

template <>
bool isEngineT< ENGINES_BEGIN >( [[maybe_unused]] const std::filesystem::path& path )
{
	return false;
}

template <>
bool isEngineT< ENGINES_END >( [[maybe_unused]] const std::filesystem::path& path )
{
	return true;
}

constexpr std::tuple< std::string_view, std::string_view > blacklist_execs { "UnityCrashHandler32.exe",
	                                                                         "UnityCrashHandler64.exe" };

bool isBlacklistT( const std::string& name, const std::string_view comp )
{
	return name == comp;
}

bool isBlacklistT( const std::string& name, std::string_view comp, std::same_as<std::string_view> auto... comps )
{
	return name == comp || isBlacklistT( name, comps... );
}

bool isBlacklist( const std::string& name )
{
	ZoneScoped;
	// In order to 'inject' name into the arguments we have to create a function
	// to unpack the parameter pack and slam it straight back into the function.
	auto func = [ &name ]( auto&&... args ) noexcept
	{ return isBlacklistT( name, std::forward< decltype( args ) >( args )... ); };

	return std::apply( func, blacklist_execs );
}

std::vector< std::filesystem::path > detectExecutables( const std::filesystem::path& path )
{
	ZoneScoped;
	std::vector< std::filesystem::path > potential_executables;

	//Check for a valid game executable in the folder
	for ( const auto& file : std::filesystem::directory_iterator( path ) )
	{
		if ( file.is_regular_file() )
		{
			const auto ext { file.path().extension() };

			if ( isBlacklist( file.path().filename().string() ) ) continue;

			QMimeDatabase mime_db;
			const auto type {
				mime_db.mimeTypeForFile( QString::fromStdString( file.path().string() ), QMimeDatabase::MatchContent )
			};

			//General executables
			if ( type.inherits( "application/x-ms-dos-executable" ) && ext == ".exe" )
			{
				potential_executables.emplace_back( std::filesystem::relative( file, path ) );
				continue;
			}
			else if ( type.inherits( "text/html" ) && file.path().filename() == "index.html" )
			{
				potential_executables.emplace_back( std::filesystem::relative( file, path ) );
				continue;
			}

			if constexpr ( sys::is_unix )
			{
				if ( type.inherits( "application/x-shellscript" ) && ext == ".sh" )
				{
					potential_executables.emplace_back( std::filesystem::relative( file, path ) );
					continue;
				}
			}
		}
	}

	return scoreExecutables( std::move( potential_executables ) );
}

/**
 * @warning Provides no MIME checking. CHECK YOURSELF
 * @param paths
 * @return
 */
std::vector< std::filesystem::path >
	scoreExecutables( std::vector< std::filesystem::path > paths, [[maybe_unused]] const Engine engine_type )
{
	ZoneScoped;
	std::vector< std::pair< std::filesystem::path, int > > execs;

	for ( auto& path : paths )
	{
		if constexpr ( sys::is_unix )
			if ( path.extension() == ".sh" ) execs.emplace_back( std::move( path ), 20 );

		if ( path.extension() == ".exe" ) execs.emplace_back( std::move( path ), sys::is_unix ? 10 : 20 );
	}

	std::sort(
		execs.begin(),
		execs.end(),
		[]( const auto& first, const auto& second ) { return first.second > second.second; } );

	std::vector< std::filesystem::path > sorted_paths;
	for ( auto& [ path, score ] : execs ) sorted_paths.emplace_back( std::move( path ) );

	return sorted_paths;
}

template <>
QString engineNameT< UNKNOWN >()
{
	return "Unknown";
}

template < Engine engine >
Engine findEngine( const std::filesystem::path& path )
{
	ZoneScoped;
	if constexpr ( engine == ENGINES_END )
		return UNKNOWN;
	else
	{
		if ( isEngineT< engine >( path ) )
			return engine;
		else
			return findEngine< static_cast< Engine >( engine + 1 ) >( path );
	}
}

Engine determineEngine( const std::filesystem::path& path )
{
	ZoneScoped;
	return findEngine< ENGINES_BEGIN >( path );
}

template < Engine engine_t >
QString getEngineNameT( const Engine engine )
{
	ZoneScoped;
	if constexpr ( engine_t == ENGINES_END )
		return engineNameT< UNKNOWN >();
	else if constexpr ( engine_t == ENGINES_BEGIN )
		return getEngineNameT< static_cast< Engine >( ENGINES_BEGIN + 1 ) >( engine );
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
bool isEngineT< RenPy >( const std::filesystem::path& path )
{
	ZoneScoped;
	return std::filesystem::exists( path / "renpy" );
}

template <>
QString engineNameT< RenPy >()
{
	return "Ren'Py";
}

template <>
bool isEngineT< Unity >( const std::filesystem::path& path )
{
	ZoneScoped;
	return std::filesystem::exists( path / "Data" / "Managed" / "Assembly-CSharp.dll" );
}

template <>
QString engineNameT< Unity >()
{
	return "Unity";
}

template <>
bool isEngineT< Unreal >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< Unreal >()
{
	return "Unreal";
}

template <>
bool isEngineT< RPGM >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< RPGM >()
{
	return "RPG Maker";
}

template <>
bool isEngineT< WolfRPG >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< WolfRPG >()
{
	return "Wolf RPG";
}

template <>
bool isEngineT< HTML >( const std::filesystem::path& path )
{
	ZoneScoped;
	return std::filesystem::exists( path / "index.html" );
}

template <>
QString engineNameT< HTML >()
{
	return "HTML";
}

template <>
bool isEngineT< VisualNovelMaker >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< VisualNovelMaker >()
{
	return "Visual Novel Maker";
}

template <>
bool isEngineT< TyanoBuilder >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;

	return std::filesystem::exists( path / "resources" / "app" / "tyrano" );
}

template <>
QString engineNameT< TyanoBuilder >()
{
	return "TyanoBuilder Visual Novel Software";
}

template <>
bool isEngineT< Java >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< Java >()
{
	return "Java";
}

template <>
bool isEngineT< Flash >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< Flash >()
{
	return "Flash";
}

template <>
bool isEngineT< RAGS >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< RAGS >()
{
	return "RAGS";
}

template <>
bool isEngineT< KiriKiri >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< KiriKiri >()
{
	return "KiriKiri";
}

template <>
bool isEngineT< NScripter >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< NScripter >()
{
	return "NScripter";
}

template <>
bool isEngineT< NVList >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< NVList >()
{
	return "NVList";
}

template <>
bool isEngineT< Sukai2 >( [[maybe_unused]] const std::filesystem::path& path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< Sukai2 >()
{
	return "Sukai2";
}
