//
// Created by kj16609 on 1/20/23.
//

#ifndef HYDRUS95_PATHMANAGER_HPP
#define HYDRUS95_PATHMANAGER_HPP


/**

Valid placeholders:

{h95_data}
{title}
{creator}
{engine}
{version}
*/

#include "h95/config.hpp"
#include "KeyReplacer.hpp"
#include <string>
#include <filesystem>
#include <iostream>
#include <tracy/Tracy.hpp>

struct PathManager
{
	KeyReplacer key_replacer {};

	std::filesystem::path root {};

	PathManager();

	inline std::filesystem::path fillPath( const std::filesystem::path& path, bool relativeFromRoot = false ) const
	{
		ZoneScoped;
		QString path_str { QString::fromStdString( path.string() ) };

		//Example : {h95_data}/{engine}/{version}
		//Example : {path}

		key_replacer.replaceKeys(path_str);

		return {path_str.toStdString()};
	}

	inline std::filesystem::path relative( const std::filesystem::path& path ) const
	{
		ZoneScoped;
		return std::filesystem::relative( path, root );
	}

	inline void populateValues( const std::filesystem::path& path, const std::filesystem::path key_path )
	{
		ZoneScoped;
		auto temp_path { path };
		auto temp_key_path { key_path };

		std::vector< std::pair< QString, QString > > temp_values;

		uint16_t step_counter { 0 };
		while ( !temp_key_path.empty() && step_counter < 512 )
		{
			std::cout << "path: " << temp_path << std::endl;

			const auto key { QString::fromStdString( temp_key_path.filename().string() ) };
			temp_key_path = temp_key_path.parent_path();
			if ( temp_key_path.empty() )
				temp_values.emplace_back( key, QString::fromStdString( temp_path.string() ) );
			else
				temp_values.emplace_back( key, QString::fromStdString( temp_path.filename().string() ) );

			temp_path = temp_path.parent_path();
			++step_counter;
		}

		for ( const auto& [key, value] : temp_values )
		{
			key_replacer.registerKey( key, value );
		}
	}

	inline void setRoot( const std::filesystem::path& path ) { root = path; }
};

#endif	//HYDRUS95_PATHMANAGER_HPP
