//
// Created by kj16609 on 1/31/23.
//


#include "PathManager.hpp"

PathManager::PathManager()
{
	key_replacer.registerKey(
		"{h95_games}",
		QString::fromStdString(
			std::filesystem::canonical( getSettings< QString >( "paths/games", "./data/games" ).toStdString() )
				.string() ) );

	key_replacer.registerKey(
		"{h95_data}",
		QString::fromStdString(
			std::filesystem::canonical( getSettings< QString >( "path/data", "./data" ).toStdString() ).string() ) );
}

std::filesystem::path PathManager::fillPath( const std::filesystem::path& path, [[maybe_unused]] bool relativeFromRoot )
	const
{
	ZoneScoped;
	QString path_str { QString::fromStdString( path.string() ) };

	//Example : {h95_data}/{engine}/{version}
	//Example : {path}

	key_replacer.replaceKeys( path_str );

	return { path_str.toStdString() };
}

//! Uses `key_path` to rip values out of `path` to populate `key_replacer`
void PathManager::populateValues( const std::filesystem::path& path, const std::filesystem::path key_path )
{
	ZoneScoped;
	auto temp_path { path };
	auto temp_key_path { key_path };

	std::vector< std::pair< QString, QString > > temp_values;

	uint16_t step_counter { 0 };
	while ( !temp_key_path.empty() && step_counter < 512 )
	{
		const auto key { QString::fromStdString( temp_key_path.filename().string() ) };
		temp_key_path = temp_key_path.parent_path();
		if ( temp_key_path.empty() )
			temp_values.emplace_back( key, QString::fromStdString( temp_path.string() ) );
		else
			temp_values.emplace_back( key, QString::fromStdString( temp_path.filename().string() ) );

		temp_path = temp_path.parent_path();
		++step_counter;
	}

	for ( const auto& [key, value] : temp_values ) { key_replacer.registerKey( key, value ); }
}
