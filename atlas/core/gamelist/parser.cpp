//
// Created by kj16609 on 6/4/23.
//

#include <QRegularExpression>
#include <QSettings>

#include "core/database/RapidTransaction.hpp"
#include "core/database/remote/F95Data.hpp"
#include "core/logging/logging.hpp"
#include "utils.hpp"

namespace gl
{

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
			constexpr auto regex_str { R"(^.*\/f95zone.(to|com)\/threads\/(?P<f95_id>[0-9]*)(\/$|$))" };
			QRegularExpression regex { regex_str };
			assert( regex.isValid() );
			const auto match { regex.match( infos.thread_url ) };
			if ( !match.isValid() || match.captured( "f95_id" ) == "" )
			{
				atlas::logging::warn(
					"Unable to extract f95 id from GL_Infos.ini for {} using regex \"{}\" and url \"{}\"",
					path,
					regex_str,
					infos.thread_url );
				return infos;
			}
			else
			{
				infos.f95_thread_id = ( match.captured( "f95_id" ).toULongLong() );
			}
		}

		return infos;
	}
	catch ( std::exception& e )
	{
		atlas::logging::error( "Failed to parse GL_Infos.ini at {}. Reason: {}", path, e.what() );
		return {};
	}

	[[nodiscard]] AtlasID parseAtlas( const std::filesystem::path& path )
	try
	{
		const GameListInfos data { parse( path ) };
		if ( data.f95_thread_id == INVALID_F95_ID || data.thread_url.isEmpty() ) return INVALID_ATLAS_ID;

		AtlasID id { INVALID_ATLAS_ID };

		RapidTransaction() << "SELECT atlas_id FROM f95_zone_data WHERE f95_id = ?" << data.f95_thread_id >> id;

		return id;
	}
	catch ( std::exception& e )
	{
		atlas::logging::warn( "Failed to parse f95 data! {}", e.what() );
		return INVALID_ATLAS_ID;
	}

} // namespace gl
