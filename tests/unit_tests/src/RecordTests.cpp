//
// Created by kj16609 on 1/15/23.
//

#include <gtest/gtest.h>

#include <backend/Record.hpp>
#include <backend/database.hpp>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QJsonArray>
#include <fstream>
#include "backend/f95parser.hpp"

class RecordCreation : public ::testing::Test
{
	int argc { 0 };
	char** argv { nullptr };

	QApplication app { argc, argv };

	inline static bool failed_last { false };

	void SetUp() override
	{
		if ( failed_last ) GTEST_SKIP() << "Skipping due to failed test previously";

		database::initalize();
	}

	void TearDown() override
	{
		if ( this->HasFailure() || this->IsSkipped() )
		{
			failed_last = true;
			return;
		}

		std::filesystem::remove( "./data/config.ini" );
		std::filesystem::remove( "./data/hydrus95.db" );
	}
};

class CachePages : public ::testing::Test
{
	int argc { 0 };
	char** argv { nullptr };

	QApplication app { argc, argv };
};

TEST_F( RecordCreation, GameRecord )
{
	GameMetadata g_metadata;
	g_metadata.creator_name = "Test";
	g_metadata.executable_path = "no.exe";
	g_metadata.folder_path = "your/moms/house";
	g_metadata.h95_ownerhip = false;
	g_metadata.version_string = "v1.96.420";

	Record record { Record::create( GAME_RECORD, { g_metadata } ) };
	ASSERT_GT( record.m_id, 0 );

	Record db_record { Record::select( record.m_id ) };
	ASSERT_EQ( record, db_record );
}

TEST_F( RecordCreation, GameRecordIncomplete )
{
	GameMetadata g_metadata;
	g_metadata.creator_name = "Test";
	g_metadata.version_string = "v1.0";

	Record record { Record::create( GAME_RECORD, { g_metadata } ) };
	ASSERT_GT( record.m_id, 0 );

	Record db_record { Record::select( record.m_id ) };
	ASSERT_EQ( record, db_record );
}

TEST_F( CachePages, CacheJSON )
{
#ifndef TEST_NET
	GTEST_SKIP() << "Skipping due to network tests not being allowed";
#endif

	if ( std::filesystem::exists( "./cached" ) )
		GTEST_SKIP() << "Skipping due to pages already cached";
	else
		std::filesystem::create_directories( "./cached" );

	uint64_t page { 1 };
	uint64_t total { 200 };

	const QString source { "https://f95zone.to/sam/latest_alpha/latest_data.php?" };

	QNetworkAccessManager manager;

	const auto start { std::chrono::steady_clock::now() };

	std::vector< QJsonObject > json_collected;

	auto fetchPage = [&source, &manager]( const int page ) -> QByteArray
	{
		QUrlQuery query { source };
		query.addQueryItem( "cmd", "list" );
		query.addQueryItem( "cat", "games" );
		query.addQueryItem( "sort", "date" );
		query.addQueryItem( "rows", "90" );
		query.addQueryItem( "page", QString::number( page ) );

		QNetworkRequest request { query.query() };
		QNetworkReply* reply { manager.get( request ) };

		QEventLoop loop;
		QObject::connect( reply, SIGNAL( finished() ), &loop, SLOT( quit() ) );
		std::cout << "Waiting on loop " << std::endl;
		loop.exec();
		std::cout << "Loop done" << std::endl;

		if ( reply->error() != QNetworkReply::NoError ) { throw std::runtime_error( "Network fucked up" ); }

		const QByteArray data { reply->readAll() };
		delete reply;

		return data;
	};

	while ( page <= total )
	{
		try
		{
			const auto data { fetchPage( page ) };
			QJsonDocument json_doc { QJsonDocument::fromJson( data ) };
			json_collected.emplace_back( json_doc.object()["msg"].toObject() );

			total = json_doc["msg"].toObject()["pagination"].toObject()["total"].toInt();

			std::cout << "Collected page " << page << std::endl;

			using namespace std::chrono_literals;

			if ( std::ofstream ofs(
					 "./cached/f95-json-" + std::to_string( page ) + ".json",
					 std::ios::binary | std::ios::out );
				 ofs )
				ofs.write( data.data(), data.size() );
			else
				FAIL() << "Failed to open location for json cache";

			if ( page % 5 == 0 )
				std::this_thread::sleep_for( 1min );
			else
				std::this_thread::sleep_for( 2s );
		}
		catch ( std::exception& e )
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for( 10min );
			continue;
		}

		++page;
	}

	//Quickly fetch the first page again just incase new results were added
	if ( std::ofstream ofs( "./cached/f95-json-first" + std::to_string( 0 ) + ".json" ); ofs )
	{
		const auto data { fetchPage( 1 ) };
		ofs.write( data.data(), data.size() );
	}

	std::cout << "Collected " << page << " pages" << std::endl;

	const auto end { std::chrono::steady_clock::now() };

	std::cout << "Collected in: " << ( end - start ) << std::endl;
}

TEST_F( RecordCreation, MassCreation )
{
#ifndef TEST_NET
	GTEST_SKIP() << "Skipping due to network tests not being allowed";
#endif

	for ( const auto& file : std::filesystem::directory_iterator( "./cached" ) )
	{
		std::cout << "Scanning file " << file << std::endl;

		QByteArray data;
		data.resize( std::filesystem::file_size( file ) );

		if ( std::ifstream ifs( file.path(), std::ios::binary ); ifs )
			ifs.read( data.data(), data.size() );
		else
			FAIL() << "Failed to open file";

		QJsonParseError error;
		const QJsonDocument json { QJsonDocument::fromJson( data, &error ) };

		if ( error.error != QJsonParseError::NoError ) throw std::runtime_error( "EVEN FUCKER" );

		if ( !json.object().contains( "msg" ) || !json.object()["msg"].isObject() )
			throw std::runtime_error( "No object" );

		const QJsonObject msg { json["msg"].toObject() };

		if ( !msg.contains( "data" ) || !msg["data"].isArray() ) throw std::runtime_error( "No array" );

		const QJsonArray record_list = msg["data"].toArray();

		for ( const auto& record_raw : record_list )
		{
			const auto& record { record_raw.toObject() };
			const F95Parser parser { record };

			const auto& [thread_id, title, creator, version, prefixes, tags, cover, previews, new_entry] = parser;

			GameMetadata metadata;
			metadata.creator_name = creator;
			metadata.version_string = version;
			metadata.game_name = title;

			Record new_record { Record::create( GAME_RECORD, { metadata }, cover, previews ) };
		}
	}
}
