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

constexpr std::tuple blacklist_execs { "UnityCrashHandler32.exe", "UnityCrashHandler64.exe" };

template < typename T >
bool isBlacklistT( const std::string& name, T comp )
{
	return name == comp;
}

template < typename T, typename... Ts >
bool isBlacklistT( const std::string& name, T comp, Ts... comps )
{
	return name == comp || isBlacklistT( name, comps... );
}

//Formatter eats shit here
bool isBlacklist( const std::string& name )
{
	ZoneScoped;
	// In order to 'inject' name into the arguments we have to create a function
	// to unpack the parameter pack and slam it straight back into the function.
	auto func = [ &name ]( auto&&... args ) noexcept
	{ return isBlacklistT( name, std::forward< const char* const >( args )... ); };

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
Engine findEngine( const std::filesystem::path& path, const std::filesystem::path& executable_path )
{
	ZoneScoped;
	if constexpr ( engine == ENGINES_END )
		return UNKNOWN;
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
	ZoneScoped;
	return findEngine< ENGINES_BEGIN >( path, executable_path );
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
bool isEngine<
	RenPy >( const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine<
	Unity >( const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine< Unreal >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine< RPGM >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine< WolfRPG >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine<
	HTML >( const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine< VisualNovelMaker >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine< TyanoBuilder >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
{
	ZoneScoped;

	return std::filesystem::exists(path / "resources" / "app" / "tyrano" );
}

template <>
QString engineNameT< TyanoBuilder >()
{
	return "TyanoBuilder Visual Novel Software";
}

template <>
bool isEngine< Java >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine<
	Flash >( [[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine< RAGS >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine< KiriKiri >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine< NScripter >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine< NVList >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
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
bool isEngine< Sukai2 >(
	[[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::filesystem::path& executable_path )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< Sukai2 >()
{
	return "Sukai2";
}
