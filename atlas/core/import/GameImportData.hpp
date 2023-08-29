//
// Created by kj16609 on 6/5/23.
//

#ifndef ATLASGAMEMANAGER_GAMEIMPORTDATA_HPP
#define ATLASGAMEMANAGER_GAMEIMPORTDATA_HPP

#include <QString>

#include <filesystem>

#include "core/config.hpp"

struct GameImportData
{
	std::filesystem::path path;
	QString title;
	QString creator;
	QString engine;
	QString version;
	std::size_t size;
	std::uint64_t file_count;
	std::vector< std::filesystem::path > executables;
	std::filesystem::path executable;
	std::array< QString, BannerType::SENTINEL > banners;
	std::vector< QString > previews;

	//Remote information
	AtlasID atlas_id { INVALID_ATLAS_ID };

	GameImportData(
		std::filesystem::path path_in,
		QString title_in,
		QString creator_in,
		QString engine_in,
		QString version_in,
		const std::size_t size_in,
		const std::uint64_t file_count_in,
		std::vector< std::filesystem::path > executables_in,
		std::filesystem::path executable_in,
		std::array< QString, BannerType::SENTINEL > banners_in,
		std::vector< QString > previews_in ) :
	  path( std::move( path_in ) ),
	  title( std::move( title_in ) ),
	  creator( std::move( creator_in ) ),
	  engine( std::move( engine_in ) ),
	  version( std::move( version_in ) ),
	  size( size_in ),
	  file_count( file_count_in ),
	  executables( std::move( executables_in ) ),
	  executable( std::move( executable_in ) ),
	  banners( std::move( banners_in ) ),
	  previews( std::move( previews_in ) )
	{}

	GameImportData() = delete;
	GameImportData( GameImportData&& other ) = default;
	GameImportData( const GameImportData& other ) = default;
};

#endif //ATLASGAMEMANAGER_GAMEIMPORTDATA_HPP
