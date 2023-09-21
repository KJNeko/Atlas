//
// Created by kj16609 on 1/12/23.
//

#include "Database.hpp"

#include <QImage>

#include <sqlite3.h>

#include "RapidTransaction.hpp"
#include "core/config.hpp"
#include "core/database/migrations/templates.hpp"
#include "core/database/record/game/Game.hpp"

namespace internal
{
	static sqlite3* db_handle { nullptr };
	static std::mutex db_mtx;
} // namespace internal

[[nodiscard]] sqlite3& Database::ref()
{
	if ( internal::db_handle != nullptr )
		return *internal::db_handle;
	else
		throw DatabaseException( "Database was not initalized!" );
}

[[nodiscard]] internal::MtxType& Database::lock()
{
	return internal::db_mtx;
}

void Database::initalize( const std::filesystem::path init_path )
{
	ZoneScoped;
	atlas::logging::init();

	if ( init_path.parent_path() != "" && !std::filesystem::exists( init_path.parent_path() ) )
		std::filesystem::create_directories( init_path.parent_path() );

	const int ret_code { sqlite3_open_v2(
		init_path.string().c_str(), &internal::db_handle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr ) };

	if ( ret_code != SQLITE_OK )
	{
		atlas::logging::critical( "Failed to load sqlite!" );
		std::abort();
	}

	RapidTransaction transaction {};

	atlas::database::migrations::runUp();

	//Add dummy record
	const QString test_name { "Galaxy Crossing: First Conquest" };
	const QString test_creator { "Atlas Games" };
	const QString test_engine { "Unity" };

	if ( !atlas::records::recordExists( test_name, test_creator, test_engine ) )
	{
		atlas::records::Game game { atlas::records::importRecord( test_name, test_creator, test_engine ) };
		game.addVersion(
			"Chapter: 1",
			"C:/Atlas Games/Galaxy Crossing First Conquest",
			"Galaxy Crossing First Conquest.exe",
			0,
			true );

		QImage banner_image { ":/images/assets/Grid_Capsule_Default.webp" };
		if ( banner_image.isNull() ) throw AtlasException( "Failed to open image asset for Grid Capsule Default" );
		std::filesystem::create_directories( "./data/images" );
		if ( !banner_image.save( "./data/images/config_image.webp" ) )
			throw AtlasException( "Failed to save Grid Capsule Default image to temporary location" );

		game.setBanner( "./data/images/config_image.webp", Normal );
	}

	config::db::first_start::set( false );
}

void Database::deinit()
{
	std::lock_guard guard { internal::db_mtx };
	sqlite3_close_v2( internal::db_handle );
}
