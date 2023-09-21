//
// Created by kj16609 on 6/5/23.
//

#ifndef ATLASGAMEMANAGER_GAMEIMPORTDATA_HPP
#define ATLASGAMEMANAGER_GAMEIMPORTDATA_HPP

#include <QString>

#include <filesystem>

#include "core/config.hpp"
#include "core/database/record/GameData.hpp"
#include "core/database/record/game/Game.hpp"
#include "core/gamelist/utils.hpp"

struct GameImportData
{
	std::filesystem::path relative_path;
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

	//GL Info
	gl::GameListInfos infos;

	//Remote information
	RecordID game_id;
	AtlasID atlas_id;
	bool conflicting_version;

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
		std::vector< QString > previews_in,
		gl::GameListInfos gl_info_in,
		RecordID record_id_in,
		AtlasID atlas_id_in ) :
	  relative_path( std::move( path_in ) ),
	  title( std::move( title_in ) ),
	  creator( std::move( creator_in ) ),
	  engine( std::move( engine_in ) ),
	  version( std::move( version_in ) ),
	  size( size_in ),
	  file_count( file_count_in ),
	  executables( std::move( executables_in ) ),
	  executable( std::move( executable_in ) ),
	  banners( std::move( banners_in ) ),
	  previews( std::move( previews_in ) ),
	  infos( gl_info_in ),
	  game_id( record_id_in ),
	  atlas_id( atlas_id_in ),
	  conflicting_version( game_id != INVALID_RECORD_ID && atlas::records::Game( game_id ).hasVersion( version ) )
	{}

	GameImportData() = delete;
	GameImportData( GameImportData&& other ) = default;
	GameImportData( const GameImportData& other ) = default;
	GameImportData& operator=( const GameImportData& other ) = default;
};

#endif //ATLASGAMEMANAGER_GAMEIMPORTDATA_HPP
