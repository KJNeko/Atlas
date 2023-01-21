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
#include <string>
#include <filesystem>
#include <iostream>

struct PathManager
{
	std::filesystem::path root{};

	std::vector< std::pair< QString, QString > > values {
		{ "{h95_data}",
		  QString::fromStdString(
			  std::filesystem::canonical( getSettings< QString >( "paths/data", "./data" ).toStdString() ).string() ) } };

	inline void registerReplacement( const QString key, const QString value )
	{
		if ( auto itter = std::find_if(
				 values.begin(),
				 values.end(),
				 [&]( const std::pair< QString, QString >& pair ) { return pair.first == key; } );
			 itter != values.end() )
			itter->second = value;
		else
			values.emplace_back( key, value );
	}

	QString value( const QString& text )
	{
		if ( auto itter = std::find_if(
				 values.begin(),
				 values.end(),
				 [&]( const std::pair< QString, QString >& pair ) { return pair.first == text; } );
			 itter != values.end() )
			return itter->second;
		else
			return {};
	}

	std::filesystem::path fillPath( const std::filesystem::path& path, bool relativeFromRoot = false )
	{
		QString path_str { QString::fromStdString( path.string() ) };

		//Example : {h95_data}/{engine}/{version}
		//Example : {path}

		for ( const auto& [key, value] : values ) path_str.replace( key, value );

		if ( relativeFromRoot )
			return { std::filesystem::relative( path_str.toStdString(), root ) };
		else
			return { path_str.toStdString() };
	}

	void populateValues( const std::filesystem::path& path, const std::filesystem::path key_path )
	{
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

		std::cout << "Finished processing after " << step_counter << " steps " << std::endl;

		for ( const auto& [key, value] : temp_values )
		{
			std::cout << key.toStdString() << ": " << value.toStdString() << std::endl;
			registerReplacement( key, value );
		}
	}

	inline void setRoot( const std::filesystem::path& path ) { root = path; }
};

#endif	//HYDRUS95_PATHMANAGER_HPP
