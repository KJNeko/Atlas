//
// Created by kj16609 on 4/1/23.
//

#include "engineDetection.hpp"

#include <QDir>
#include <QDirIterator>
#include <QMimeDatabase>
#include <QString>

#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <string>

#include "core/logging/logging.hpp"
#include "core/system.hpp"
#include "core/utils/FileScanner.hpp"

template <>
bool isEngineT< ENGINES_BEGIN >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return false;
}

template <>
bool isEngineT< Engine::UNKNOWN >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return true;
}

template <>
bool isEngineT< ENGINES_END >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return true;
}

constexpr std::tuple blacklist_execs { std::string_view( "UnityCrashHandler32.exe" ),
	                                   std::string_view( "UnityCrashHandler64.exe" ),
	                                   std::string_view( "payload.exe" ),
	                                   std::string_view( "nwjc.exe" ),
	                                   std::string_view( "notification_helper.exe" ),
	                                   std::string_view( "nacl64.exe" ),
	                                   std::string_view( "chromedriver.exe" ),
	                                   std::string_view( "Squirrel.exe" ),
	                                   std::string_view( "zsynccmake.exe" ),
	                                   std::string_view( "zsync" ),
	                                   std::string_view( "pythonw.exe" ),
	                                   std::string_view( "python.exe" ),
	                                   std::string_view( "dxwebsetup" ),
	                                   std::string_view( "UE4PrereqSetup_X64.exe" ),
	                                   std::string_view( "UEPrereqSetup_x64.exe" ),
	                                   std::string_view( "dxwebsetup.exe" ) };

bool isBlacklistT( const std::string& name, const std::string_view comp )
{
	return name == comp;
}

bool isBlacklistT(
	const std::string& name, const std::string_view comp, const std::same_as< std::string_view > auto... comps )
{
	return name == comp || isBlacklistT( name, comps... );
}

bool isBlacklist( const std::string& name )
{
	// In order to 'inject' name into the arguments we have to create a function
	// to unpack the parameter pack and slam it straight back into the function.
	auto func = [ &name ]( auto&&... args ) noexcept
	{ return isBlacklistT( name, std::forward< decltype( args ) >( args )... ); };

	return std::apply( func, blacklist_execs );
}

std::vector< std::filesystem::path > detectExecutables( atlas::utils::FileScanner& scanner )
{
	ZoneScoped;
	std::vector< std::filesystem::path > potential_executables;
	const std::vector< std::string_view > extensions { ".exe", ".html", ".sh",  ".swf", ".flv",
		                                               ".jar", ".qsp",  ".bat", ".rag", ".ps1" };

	//Check for a valid game executable in the folder
	for ( const auto& [ filename, ext, path, size, depth, relative ] : scanner )
	{
		if ( depth > 1 ) break;

		if ( std::filesystem::is_regular_file( path ) )
		{
			if ( isBlacklist( filename ) ) continue;

			if ( std::find(
					 extensions.begin(),
					 extensions.end(),
					 QString::fromStdString( path.extension().string() ).toLower().toStdString() )
			     != extensions.end() )
			{
				QMimeDatabase mime_db;
				const auto type { mime_db.mimeTypeForFile( QString::fromStdString( path.string() ) ) };

				//General executables
				//.exe
				if ( type.inherits( "application/x-ms-dos-executable" ) )
				{
					potential_executables.push_back( relative );
					continue;
				}
				//.html
				else if ( type.inherits( "text/plain" ) && ext == ".html" )
				{
					potential_executables.emplace_back( relative );
					continue;
				}
				else if ( ext == ".swf" )
				{
					potential_executables.emplace_back( relative );
					continue;
				}
				else if ( ext == ".jar" )
				{
					potential_executables.emplace_back( relative );
					continue;
				}
				else if ( ext == ".qsp" )
				{
					potential_executables.emplace_back( relative );
					continue;
				}
				else if ( ext == ".bat" )
				{
					potential_executables.emplace_back( relative );
					continue;
				}
				else if ( ext == ".rag" )
				{
					potential_executables.emplace_back( relative );
					continue;
				}
				if constexpr ( sys::is_linux )
				{
					if ( type.inherits( "application/x-shellscript" ) && ext == ".sh" )
					{
						potential_executables.emplace_back( relative );
						continue;
					}
				}
			}
			else
				continue;
		}
	}

	atlas::logging::debug( "Found {} executables at {}", potential_executables.size(), scanner.path() );

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
	std::vector< std::pair< std::filesystem::path, int > > execs;

	for ( auto& path : paths )
	{
		const std::string extension { QString::fromStdString( path.extension().string() ).toLower().toStdString() };

		if ( extension == ".exe" )
		{
			if ( std::find( path.begin(), path.end(), "32" ) != path.end() ) // Executable is likely 32 bit
				execs.emplace_back( std::move( path ), sys::is_linux ? 10 : 15 );
			else
				execs.emplace_back( std::move( path ), sys::is_linux ? 10 : 20 );
		}
		else if ( extension == ".bat" )
			execs.emplace_back( std::move( path ), sys::is_linux ? 10 : 25 );
		else if ( extension == ".sh" )
			execs.emplace_back( std::move( path ), sys::is_linux ? 25 : 10 );
		else
			execs.emplace_back( std::move( path ), 10 );
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
	//Return other instead of unknown
	return "Other";
}

template < Engine engine >
Engine findEngine( atlas::utils::FileScanner& scanner )
{
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

Engine determineEngine( atlas::utils::FileScanner& scanner )
{
	return findEngine< ENGINES_BEGIN >( scanner );
}

template < Engine engine_t >
QString getEngineNameT( const Engine engine )
{
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
//Use formats for fileSets to verify if it is that specific engine type

template <>
bool isEngineT< RenPy >( atlas::utils::FileScanner& scanner )
{
	ZoneScopedN( "isEngine< RenPy >" );
	return checkEngineType( "RenPy", scanner );
}

template <>
QString engineNameT< RenPy >()
{
	return "Ren'Py";
}

template <>
bool isEngineT< Unity >( atlas::utils::FileScanner& scanner )
{
	ZoneScopedN( "isEngine< Unity >" );
	/*for ( const auto& file : scanner )
	{
		if ( file.depth > 1 ) return false;

		if ( file.filename == "Data" && std::filesystem::is_directory( file.path ) )
		{
			//Check deeper
			return std::filesystem::exists( scanner.path() / "Data" / "Managed" / "Assembly-CSharp.dll" );
		}
	}*/

	return checkEngineType( "Unity", scanner );
}

template <>
QString engineNameT< Unity >()
{
	return "Unity";
}

template <>
bool isEngineT< Unreal >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "Unreal", scanner );
}

template <>
QString engineNameT< Unreal >()
{
	return "Unreal";
}

template <>
bool isEngineT< RPGM >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "RPGMaker", scanner );
}

template <>
QString engineNameT< RPGM >()
{
	return "RPG Maker";
}

template <>
bool isEngineT< WolfRPG >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "WolfRPGEditor", scanner );
}

template <>
QString engineNameT< WolfRPG >()
{
	return "Wolf RPG";
}

template <>
bool isEngineT< HTML >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	ZoneScopedN( "isEngine< HTML >" );
	return checkEngineType( "Html", scanner );
}

template <>
QString engineNameT< HTML >()
{
	return "HTML";
}

template <>
bool isEngineT< VisualNovelMaker >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "VisualNovelMaker", scanner );
}

template <>
QString engineNameT< VisualNovelMaker >()
{
	return "Visual Novel Maker";
}

template <>
bool isEngineT< TyanoBuilder >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	ZoneScopedN( "isEngine< TyanoBuilder >" );
	return checkEngineType( "TyranoBuilder", scanner );
}

template <>
QString engineNameT< TyanoBuilder >()
{
	return "TyanoBuilder";
}

template <>
bool isEngineT< Java >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "Java", scanner );
}

template <>
QString engineNameT< Java >()
{
	return "Java";
}

template <>
bool isEngineT< Flash >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "Flash", scanner );
}

template <>
QString engineNameT< Flash >()
{
	return "Flash";
}

template <>
bool isEngineT< RAGS >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "RAGS", scanner );
}

template <>
QString engineNameT< RAGS >()
{
	return "RAGS";
}

template <>
bool isEngineT< KiriKiri >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "KiriKiri", scanner );
}

template <>
QString engineNameT< KiriKiri >()
{
	return "KiriKiri";
}

template <>
bool isEngineT< NScripter >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "NScripter", scanner );
}

template <>
QString engineNameT< NScripter >()
{
	return "NScripter";
}

template <>
bool isEngineT< NVList >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "NVList", scanner );
}

template <>
QString engineNameT< NVList >()
{
	return "NVList";
}

template <>
bool isEngineT< Sukai2 >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "Sukai2", scanner );
}

template <>
QString engineNameT< Sukai2 >()
{
	return "Sukai2";
}

template <>
bool isEngineT< MonoGame >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "MonoGame", scanner );
}

template <>
QString engineNameT< MonoGame >()
{
	return "Mono Game";
}

template <>
bool isEngineT< XNA >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "XNA", scanner );
}

template <>
QString engineNameT< XNA >()
{
	return "XNA";
}

template <>
bool isEngineT< Adobe_AIR >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "Adobe_AIR", scanner );
}

template <>
QString engineNameT< Adobe_AIR >()
{
	return "Adobe AIR";
}

template <>
bool isEngineT< QSP >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "QSP", scanner );
}

template <>
QString engineNameT< QSP >()
{
	return "QSP";
}

template <>
bool isEngineT< BAT >( [[maybe_unused]] atlas::utils::FileScanner& scanner )
{
	return checkEngineType( "BAT", scanner );
}

template <>
QString engineNameT< BAT >()
{
	return "Windows";
}

//Pass engine name for verifying type
bool checkEngineType( std::string engine, atlas::utils::FileScanner& scanner )
{
	//get current directory
	bool isEngine { false };
	const std::filesystem::path engine_path { std::filesystem::current_path() / "engine" / "types"
		                                      / ( "Engine." + engine + ".txt" ) };

	if ( std::ifstream ifs( engine_path ); ifs )
	{
		//Read in each line a store in array
		std::string line = "";

		while ( getline( ifs, line ) )
		{
			//Check if first item in string is a period for a file type
			//std::vector< char > charArry;
			//std::copy( line.begin(), line.end(), std::back_inserter( charArry ) );
			if ( line[ 0 ] == '.' ) //file type check
			{
				//Go through all files and check if extention exist
				for ( const auto& file : scanner )
				{
					if ( file.depth > 1 ) break;
					if ( file.ext == line )
					{
						isEngine = true;
						break;
					}
				}
			}
			else
			{
				//Check if there is a / at begining of string. add if missing
				if ( line[ 0 ] != '/' )
				{
					line = "\\" + line;
				}
				//Check if path is valid
				if ( std::filesystem::is_directory( scanner.path().string() + line ) )
				{
					isEngine = true;
					break;
				}
				//Check if file is valid
				if ( std::filesystem::exists( scanner.path().string() + line ) )
				{
					isEngine = true;
					break;
				}
			}
		};

		return isEngine;
	}
	else
		return false;
}