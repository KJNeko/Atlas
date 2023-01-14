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
	}


}  // namespace database