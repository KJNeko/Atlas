//
// Created by kj16609 on 4/1/23.
//

#include "engineDetection.hpp"

#include <iostream>

#include <QMimeDatabase>

#include <tracy/Tracy.hpp>

#include "../../system.hpp"
#include "core/logging.hpp"

template <>
bool isEngineT< ENGINES_BEGIN >( [[maybe_unused]] FileScanner& scanner )
{
	return false;
}

template <>
bool isEngineT< ENGINES_END >( [[maybe_unused]] FileScanner& scanner )
{
	return true;
}

constexpr std::tuple< std::string_view, std::string_view > blacklist_execs { "UnityCrashHandler32.exe",
	                                                                         "UnityCrashHandler64.exe" };

bool isBlacklistT( const std::string& name, const std::string_view comp )
{
	return name == comp;
}

bool isBlacklistT( const std::string& name, std::string_view comp, std::same_as< std::string_view > auto... comps )
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

std::vector< std::filesystem::path > detectExecutables( FileScanner& scanner )
{
	ZoneScoped;
	std::vector< std::filesystem::path > potential_executables;

	//Check for a valid game executable in the folder
	for ( const auto& [ filename, ext, path, size, depth, relative ] : scanner )
	{
		if ( depth > 1 ) break;

		if ( std::filesystem::is_regular_file( path ) )
		{
			ZoneScopedN( "Is Regular" );
			if ( isBlacklist( filename ) ) continue;

			QMimeDatabase mime_db;
			const auto type {
				mime_db.mimeTypeForFile( QString::fromStdString( path.string() ), QMimeDatabase::MatchContent )
			};

			//General executables
			if ( type.inherits( "application/x-ms-dos-executable" ) && ext == ".exe" )
			{
				//prioritize AMD64
				path.string().std::string::find( "32" ) ?
					potential_executables.insert( potential_executables.begin(), relative ) :
					potential_executables.insert( potential_executables.end(), relative );
				continue;
			}
			else if ( type.inherits( "text/html" ) && filename == "index.html" )
			{
				potential_executables.emplace_back( relative );
				continue;
			}

			if constexpr ( sys::is_unix )
			{
				if ( type.inherits( "application/x-shellscript" ) && ext == ".sh" )
				{
					potential_executables.emplace_back( relative );
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
Engine findEngine( FileScanner& scanner )
{
	ZoneScoped;
	if constexpr ( engine == ENGINES_END )
		return UNKNOWN;
	else
	{
		if ( isEngineT< engine >( scanner ) )
			return engine;
		else
			return findEngine< static_cast< Engine >( engine + 1 ) >( scanner );
	}
}

Engine determineEngine( FileScanner& scanner )
{
	ZoneScoped;
	return findEngine< ENGINES_BEGIN >( scanner );
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
bool isEngineT< RenPy >( FileScanner& scanner )
{
	ZoneScoped;

	for ( const auto& file : scanner )
	{
		if ( file.depth > 1 ) return false;

		if ( file.filename == "renpy" && std::filesystem::is_directory( file.path ) ) return true;
	}
	return false;
}

template <>
QString engineNameT< RenPy >()
{
	return "Ren'Py";
}

template <>
bool isEngineT< Unity >( FileScanner& scanner )
{
	ZoneScoped;
	for ( const auto& file : scanner )
	{
		if ( file.depth > 1 ) return false;

		if ( file.filename == "Data" && std::filesystem::is_directory( file.path ) )
		{
			//Check deeper
			return std::filesystem::exists( scanner.path() / "Data" / "Managed" / "Assembly-CSharp.dll" );
		}
	}

	return false;
}

template <>
QString engineNameT< Unity >()
{
	return "Unity";
}

template <>
bool isEngineT< Unreal >( [[maybe_unused]] FileScanner& scanner )
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
bool isEngineT< RPGM >( [[maybe_unused]] FileScanner& scanner )
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
bool isEngineT< WolfRPG >( [[maybe_unused]] FileScanner& scanner )
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
bool isEngineT< HTML >( [[maybe_unused]] FileScanner& scanner )
{
	ZoneScoped;

	bool html_found { false };

	for ( const auto& file : scanner )
	{
		if ( file.depth > 1 ) return false;

		if ( file.ext == ".exe" ) return false;
		if ( file.ext == ".html" ) html_found = true;
	}

	return html_found;
}

template <>
QString engineNameT< HTML >()
{
	return "HTML";
}

template <>
bool isEngineT< VisualNovelMaker >( [[maybe_unused]] FileScanner& scanner )
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
bool isEngineT< TyanoBuilder >( [[maybe_unused]] FileScanner& scanner )
{
	ZoneScoped;

	for ( const auto& file : scanner )
	{
		if ( file.depth > 1 ) return false;

		if ( file.filename == "resources" && std::filesystem::is_directory( file.path ) )
		{
			//Search deeper
			return std::filesystem::exists( scanner.path() / "resources" / "app" / "tyrano" );
		}
	}

	return false;
}

template <>
QString engineNameT< TyanoBuilder >()
{
	return "TyanoBuilder Visual Novel Software";
}

template <>
bool isEngineT< Java >( [[maybe_unused]] FileScanner& scanner )
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
bool isEngineT< Flash >( [[maybe_unused]] FileScanner& scanner )
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
bool isEngineT< RAGS >( [[maybe_unused]] FileScanner& scanner )
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
bool isEngineT< KiriKiri >( [[maybe_unused]] FileScanner& scanner )
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
bool isEngineT< NScripter >( [[maybe_unused]] FileScanner& scanner )
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
bool isEngineT< NVList >( [[maybe_unused]] FileScanner& scanner )
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
bool isEngineT< Sukai2 >( [[maybe_unused]] FileScanner& scanner )
{
	ZoneScoped;
	return false;
}

template <>
QString engineNameT< Sukai2 >()
{
	return "Sukai2";
}
