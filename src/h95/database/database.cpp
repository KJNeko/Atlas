//
// Created by kj16609 on 1/12/23.
//

#include "database.hpp"
#include "h95/database/Record.hpp"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <filesystem>
#include <iostream>
#include <QSqlDriver>

namespace database
{
	void initalize()
	{
		std::filesystem::create_directory( "./data" );

		if ( !QSqlDatabase::drivers().contains( "QSQLITE" ) )
		{
			QMessageBox::critical( nullptr, "Unable to load database", "This program requires the QSQLITE driver" );
			std::abort();
		}

		auto db { QSqlDatabase::addDatabase( "QSQLITE" ) };
		db.setDatabaseName( "./data/hydrus95.db" );
		db.open();

		if ( !db.isOpen() )
		{
			QMessageBox::critical( nullptr, "Failed to open database", db.lastError().text() );
			std::abort();
		}

		const std::vector< QString > table_strs {
			"CREATE TABLE records (record_id INTEGER PRIMARY KEY, title TEXT, creator TEXT, version TEXT, unique(title, creator, version))",
			"CREATE TABLE game_metadata (record_id INTEGER REFERENCES records(record_id), game_path TEXT, exec_path TEXT)",
			"CREATE TABLE previews (record_id INTEGER REFERENCES records(record_id), type TEXT, path TEXT)",
			"CREATE TABLE flags (record_id INTEGER REFERENCES records(record_id), installed INTEGER, played INTEGER, wanted INTEGER)" };

		QSqlQuery query;
		for ( const auto& query_str : table_strs ) query.exec( query_str );

		if ( !db.driver()->hasFeature( QSqlDriver::NamedPlaceholders ) )
		{
			QMessageBox::critical( nullptr, "Database error", "No named placeholder support in SQLITE driver" );
			std::abort();
		}
	}


}  // namespace database