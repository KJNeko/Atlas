//
// Created by kj16609 on 3/6/23.
//

#include "GTestBox.hpp"

#include <h95/database/Database.hpp>
#include <h95/database/Record.hpp>

#include <h95/logging.hpp>

QString humanSize( float num )
{
	QStringList list;
	list << "KB"
		 << "MB"
		 << "GB"
		 << "TB";

	QStringListIterator i( list );
	QString unit( "bytes" );

	while ( num >= 1024.0f && i.hasNext() )
	{
		unit = i.next();
		num /= 1024.0f;
	}
	return QString().setNum( num, 'f', 2 ) + " " + unit;
}


class TestMassRecords : public ::testing::Test
{

	protected:
	void SetUp() override { Database::initalize( "./data/testing.db" ); }

	void TearDown() override
	{
		Database::deinit();
		std::filesystem::remove( "./data/testing.db" );
	}
};

TEST_F( TestMassRecords, Test )
{
	const std::filesystem::path db_path { "/home/kj16609/Downloads/gmData.db" };

	sqlite::database db { db_path };

	spdlog::info(
		"DB Size Before = {}",
		humanSize( static_cast< float >( std::filesystem::file_size( "./data/testing.db" ) ) ) );

	Transaction transaction;

	db << "SELECT engine, title, banner, developer FROM F95DB" >> [&transaction](
																	  const std::string engine,
																	  const std::string title,
																	  const std::string banner,
																	  const std::string developer )
	{
		Record::create(
			QString::fromStdString( title ),
			QString::fromStdString( developer ),
			QString::fromStdString( engine ),
			{},
			banner,
			{},
			transaction );
	};

	transaction.commit();

	spdlog::info(
		"DB Size = {}",
		humanSize( static_cast< float >( std::filesystem::file_size( "./data/testing.db" ) ) ) );
}
