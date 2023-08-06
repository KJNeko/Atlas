//
// Created by kj16609 on 6/4/23.
//

#include <QSettings>

#include "core/logging.hpp"
#include "utils.hpp"

namespace gl
{

	std::vector< std::filesystem::path > searchGames( const std::filesystem::path root )
	{
		std::vector< std::filesystem::path > games;

		auto itter = std::filesystem::recursive_directory_iterator( root );

		while ( itter != std::filesystem::recursive_directory_iterator() )
		{
			if ( itter->is_regular_file() && itter->path().filename() == "GL_Infos.ini" )
			{
				games.emplace_back( itter->path().parent_path() );
				itter.pop();
			}

			++itter;
		}

		return games;
	}

	/**
	 Example INI file from GL
	 [GameList]
	 Version=S2 Ch. 1 Ep. 9
	 ID=4657
	 Name=WVM
	 Thread=https://f95zone.to/threads/35910
	 */

	//! Parses a GL_Infos.ini file for it's information
	GameListInfos parse( const std::filesystem::path& path )
	try
	{
		GameListInfos infos;

		const QSettings settings( QString::fromStdString( path.string() ), QSettings::IniFormat );

		infos.version = settings.value( "GameList/Version", "" ).toString();
		infos.gl_id = settings.value( "GameList/ID", 0 ).toULongLong();
		infos.name = settings.value( "GameList/Name", "" ).toString();
		infos.thread_url = settings.value( "GameList/Thread", "" ).toString();

		if ( !infos.thread_url.isEmpty() )
		{
			//We have a thread we can parse!
			const auto regex_str { "^.*/f95zone.(to|com)\\/threads\\/(?P<f95_id>[0-9]*)(\\/$|$)" };
			QRegularExpression regex { regex_str };
			assert( regex.isValid() );
			const auto match { regex.match( infos.thread_url ) };
			if ( !match.isValid() || match.captured( "f95_id" ) == "")
			{
				//TODO: devwarn here
				spdlog::warn(
					"Unable to extract f95 id from GL_Infos.ini for {} using regex \"{}\" and url \"{}\"",
					path,
					regex_str,
					infos.thread_url );
				return infos;
			}
			else
			{
				infos.f95_thread_id = static_cast< uint64_t >( match.captured( "f95_id" ).toULongLong() );
			}
		}
		else
			spdlog::warn( "{} did not have a thread url!", path );

		return infos;
	}
	catch ( std::exception& e )
	{
		spdlog::warn( "Failed to parse GL_Infos.ini at {}. Reason: {}", path, e.what() );
		return {};
	}
	catch ( ... )
	{
		spdlog::warn( "Failed to parse GL_Infos.ini at {}. Reason: Unknown", path );
		return {}; //Parse failed. Return empty struct
	}

} // namespace gl
