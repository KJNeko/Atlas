//
// Created by kj16609 on 6/5/23.
//

#ifndef ATLASGAMEMANAGER_GAMEIMPORTDATA_HPP
#define ATLASGAMEMANAGER_GAMEIMPORTDATA_HPP

#include <filesystem>

#include <QString>

#include "core/config.hpp"

struct GameImportData
{
	std::filesystem::path path {};
	QString title { "SOMETHING WENT WRONG" };
	QString creator { "SOMETHING WENT WRONG" };
	QString engine { "SOMETHING WENT WRONG" };
	QString version { "SOMETHING WENT WRONG" };
	std::size_t size {};
	std::vector< std::filesystem::path > executables {};
	std::filesystem::path executable {};
	std::array< QString, BannerType::SENTINEL > banners {};
	std::vector< QString > previews {};

	GameImportData(
		std::filesystem::path path_in,
		QString title_in,
		QString creator_in,
		QString engine_in,
		QString version_in,
		std::size_t size_in,
		std::vector< std::filesystem::path > executables_in,
		std::filesystem::path executable_in,
		std::array< QString, BannerType::SENTINEL > banners_in,
		std::vector< QString > previews_in ) :
	  path( std::move( path_in ) ),
	  title( std::move( title_in ) ),
	  creator( std::move( creator_in ) ),
	  engine( std::move( engine_in ) ),
	  version( std::move( version_in ) ),
	  size( std::move( size_in ) ),
	  executables( std::move( executables_in ) ),
	  executable( std::move( executable_in ) ),
	  banners( std::move( banners_in ) ),
	  previews( std::move( previews_in ) )
	{}
};

#endif //ATLASGAMEMANAGER_GAMEIMPORTDATA_HPP
