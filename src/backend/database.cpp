//
// Created by kj16609 on 1/12/23.
//

#include "database.hpp"
#include "backend/records/Record.hpp"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <filesystem>

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

		QSqlQuery query;
		query.exec(
			R"(CREATE TABLE IF NOT EXISTS subtags
			(
				subtag_id   INTEGER PRIMARY KEY,
				subtag_text TEXT
			);
				)" );

		query.exec(
			R"(CREATE TABLE IF NOT EXISTS namespaces
			(
				namespace_id   INTEGER PRIMARY KEY,
				color          INTEGER,
				namespace_text TEXT
			);
				)" );

		query.exec(
			R"(CREATE TABLE IF NOT EXISTS tags
			(
				tag_id INTEGER PRIMARY KEY,
				namespace_id REFERENCES namespaces (namespace_id),
				subtag_id REFERENCES subtags (subtag_id)
			);
				)" );

		query.exec(
			R"(
			CREATE TABLE IF NOT EXISTS records
			(
				uid INTEGER PRIMARY KEY,
				record_name TEXT,
				record_desc TEXT,
				record_notes TEXT,
				record_path TEXT
			);
				)" );

		query.exec(
			R"(CREATE TABLE IF NOT EXISTS game_records
			(
				uid      INTEGER PRIMARY KEY REFERENCES records(uid),
				chapters TEXT [],
				version  TEXT,
				creator  TEXT,
				tags     INTEGER [] REFERENCES tags (tag_id),
				sources  TEXT []
			);
				)" );
	}

	Record createEmptyRecord()
	{
		QSqlQuery query;
		query.exec( "INSERT INTO records (record_name, record_desc) VALUES ('A blank record','A newly created blank record') RETURNING uid;");
		query.first();
		return {static_cast<RecordID>(query.value(0).value<int>())};
	}


}  // namespace h95::database