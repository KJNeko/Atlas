//
// Created by kj16609 on 6/4/23.
//

#include <QSettings>

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

		infos.version = settings.value( "GameList/Version" ).toString();
		infos.f95_id = settings.value( "GameList/ID" ).toULongLong();
		infos.name = settings.value( "GameList/Name" ).toString();
		infos.thread_url = settings.value( "GameList/Thread" ).toString();

		return infos;
	}
	catch ( ... )
	{
		return {}; //Parse failed. Return empty struct
	}

} // namespace gl
