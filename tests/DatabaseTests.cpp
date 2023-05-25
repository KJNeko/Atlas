//
// Created by kj16609 on 5/17/23.
//

#include <QApplication>
#include <QFile>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include "core/database/Database.hpp"
#include "core/database/GameMetadata.hpp"
#include "core/database/Record.hpp"

TEST_CASE( "Database Init Memory", "[database]" )
{
	REQUIRE_NOTHROW( Database::initalize( ":memory:" ) );
}

TEST_CASE( "Database Init", "[database]" )
{
	std::filesystem::remove( "test.db" );
	REQUIRE_NOTHROW( Database::initalize( "test.db" ) );
	REQUIRE( std::filesystem::exists( "test.db" ) );
	REQUIRE_NOTHROW( Database::deinit() );
	REQUIRE( std::filesystem::remove( "test.db" ) );
}

TEST_CASE( "Record", "[database][record]" )
{
	int argc { 0 };
	char** argv { nullptr };

	QApplication app { argc, argv };

	REQUIRE_NOTHROW( Database::initalize( ":memory:" ) );

	SECTION( "Create record" )
	{
		const Record record { importRecord( "test", "test", "test" ) };

		SECTION( "title" )
		{
			SECTION( "getTitle" )
			{
				REQUIRE( record->getTitle() == "test" );
			}

			SECTION( "setTitle" )
			{
				REQUIRE_NOTHROW( record->setTitle( "test2" ) );
				REQUIRE( record->getTitle() == "test2" );
			}
		}

		SECTION( "creator" )
		{
			SECTION( "getCreator" )
			{
				REQUIRE( record->getCreator() == "test" );
			}

			SECTION( "setCreator" )
			{
				REQUIRE_NOTHROW( record->setCreator( "test2" ) );
				REQUIRE( record->getCreator() == "test2" );
			}
		}

		SECTION( "engine" )
		{
			SECTION( "getEngine" )
			{
				REQUIRE( record->getEngine() == "test" );
			}

			SECTION( "setEngine" )
			{
				REQUIRE_NOTHROW( record->setEngine( "test2" ) );
				REQUIRE( record->getEngine() == "test2" );
			}
		}

		SECTION( "lastPlayed" )
		{
			SECTION( "getLastPlayed" )
			{
				REQUIRE( record->getLastPlayed() == 0 );
			}

			SECTION( "setLastPlayed" )
			{
				REQUIRE_NOTHROW( record->setLastPlayed( 1 ) );
				REQUIRE( record->getLastPlayed() == 1 );
			}
		}

		SECTION( "playtime" )
		{
			SECTION( "getTotalPlaytime" )
			{
				REQUIRE( record->getTotalPlaytime() == 0 );
			}

			SECTION( "setTotalPlaytime" )
			{
				REQUIRE_NOTHROW( record->setTotalPlaytime( 1 ) );
				REQUIRE( record->getTotalPlaytime() == 1 );
			}
		}

		SECTION( "versions" )
		{
			REQUIRE_NOTHROW( record->addVersion( "v1.0", "C:\\games\\my_game", "game.exe", 1234, true ) );
			std::this_thread::
				sleep_for( std::chrono::seconds( 2 ) ); //Delay to prevent games being added in the same second
			REQUIRE_NOTHROW( record->addVersion( "v2.0", "new_game", "game.exe", 1234, true ) );

			SECTION( "getVersion" )
			{
				REQUIRE( record->getVersion( "v1.0" ).has_value() );
			}

			SECTION( "getVersions" )
			{
				REQUIRE( record->getVersions().size() == 2 );
			}

			SECTION( "getLatestVersion" )
			{
				const auto version { record->getLatestVersion() };
				REQUIRE( version.has_value() );
				REQUIRE( version.value().getVersionName() == "v2.0" );
			}

			SECTION( "removeVersion" )
			{
				const auto version { record->getVersion( "v1.0" ) };
				REQUIRE_NOTHROW( record->removeVersion( version.value() ) );
				REQUIRE( record->getVersions().size() == 1 );
			}

			SECTION( "GameMetadata" )
			{
				auto version { record->getVersion( "v1.0" ) };

				SECTION( "addPlaytime" )
				{
					REQUIRE_NOTHROW( version->addPlaytime( 100 ) );
					SECTION( "getPlaytime" )
					{
						REQUIRE( version->getPlaytime() == 100 );
					}
				}

				SECTION( "setLastPlayed" )
				{
					REQUIRE_NOTHROW( version->setLastPlayed( 100 ) );
					SECTION( "getLastPlayed" )
					{
						REQUIRE( version->getLastPlayed() == 100 );
					}
				}

				SECTION( "playGame" )
				{
					//todo
					REQUIRE( false );
				}

				SECTION( "setVersionName" )
				{
					REQUIRE_NOTHROW( version->setVersionName( "v1.1" ) );
					SECTION( "getVersionName" )
					{
						REQUIRE( version->getVersionName() == "v1.1" );
					}
				}

				SECTION( "setVersionName - Fail" )
				{
					REQUIRE_THROWS( version->setVersionName( "v2.0" ) );
				}

				SECTION( "isInPlace" )
				{
					REQUIRE( version->isInPlace() );
				}

				SECTION( "getPath" )
				{
					REQUIRE( version->getPath() == "C:\\games\\my_game" );
				}

				SECTION( "getRelativeExecPath" )
				{
					REQUIRE( version->getRelativeExecPath() == "game.exe" );
				}

				SECTION( "getExecPath" )
				{
					REQUIRE( version->getExecPath() == "C:\\games\\my_game/game.exe" );
				}

				SECTION( "getFolderSize" )
				{
					REQUIRE( version->getFolderSize() == 1234 );
				}

				SECTION( "getParentID" )
				{
					REQUIRE( version->getParentID() == record->getID() );
				}
			}
		}

		SECTION( "banners" )
		{
			//Prepare by saving image
			QFile file { ":/test/assets/testfiles/test1.png" };
			file.copy( "./test.png" );
			REQUIRE( std::filesystem::exists( "./test.png" ) );

			SECTION( "Normal" )
			{
				SECTION( "setBanner" )
				{
					REQUIRE_NOTHROW( record->setBanner( "test.png", Normal ) );
					REQUIRE( std::filesystem::exists( record->getBannerPath( Normal ) ) );
					SECTION( "getBanner" )
					{
						REQUIRE( !record->getBanner( Normal ).isNull() );
					}

					std::filesystem::remove( record->getBannerPath( Normal ) );
				}
			}

			SECTION( "Wide" )
			{
				SECTION( "setBanner" )
				{
					REQUIRE_NOTHROW( record->setBanner( "test.png", Wide ) );
					REQUIRE( std::filesystem::exists( record->getBannerPath( Wide ) ) );
					SECTION( "getBanner" )
					{
						REQUIRE( !record->getBanner( Wide ).isNull() );
					}
					std::filesystem::remove( record->getBannerPath( Wide ) );
				}
			}

			SECTION( "Cover" )
			{
				SECTION( "setBanner" )
				{
					REQUIRE_NOTHROW( record->setBanner( "test.png", Cover ) );
					REQUIRE( std::filesystem::exists( record->getBannerPath( Cover ) ) );
					SECTION( "getBanner" )
					{
						REQUIRE( !record->getBanner( Cover ).isNull() );
					}
					std::filesystem::remove( record->getBannerPath( Cover ) );
				}
			}

			SECTION( "Logo" )
			{
				SECTION( "setBanner" )
				{
					REQUIRE_NOTHROW( record->setBanner( "test.png", Logo ) );
					REQUIRE( std::filesystem::exists( record->getBannerPath( Logo ) ) );
					SECTION( "getBanner" )
					{
						REQUIRE( !record->getBanner( Logo ).isNull() );
					}
					std::filesystem::remove( record->getBannerPath( Logo ) );
				}
			}

			SECTION( "getBannerScaled" )
			{
				//todo: Implement tests for each aspect_ratio via testing hash.
				REQUIRE( false );
			}

			std::filesystem::remove( "test.png" );
		}

		SECTION( "previews" )
		{
			const auto image { QImage( ":/test/assets/testfiles/test1.png" ) };
			const auto image2 { QImage( ":/test/assets/testfiles/test2.png" ) };
			std::filesystem::remove( "test.png" );
			REQUIRE_NOTHROW( image.save( "test.png", "png" ) );
			REQUIRE_NOTHROW( image2.save( "test2.png", "png" ) );

			SECTION( "getPreviewPaths" )
			{
				REQUIRE( record->getPreviewPaths().size() == 0 );
				SECTION( "getPreviews" )
				{
					REQUIRE( record->getPreviews().size() == 0 );
				}
			}

			SECTION( "addPreview" )
			{
				REQUIRE_NOTHROW( record->addPreview( "test.png" ) );
				REQUIRE( record->getPreviewPaths().size() == 1 );
				REQUIRE( std::filesystem::exists( record->getPreviewPaths()[ 0 ] ) );
			}

			SECTION( "removePreview" )
			{
				REQUIRE_NOTHROW( record->addPreview( "test.png" ) );
				REQUIRE( record->getPreviews().size() == 1 );
				const auto preview_path { record->getPreviewPaths()[ 0 ] };
				REQUIRE_NOTHROW( record->removePreview( preview_path ) );
				REQUIRE( record->getPreviews().size() == 0 );
			}

			SECTION( "reorderPreviews" )
			{
				REQUIRE_NOTHROW( record->addPreview( "test.png" ) );
				REQUIRE_NOTHROW( record->addPreview( "test2.png" ) );

				REQUIRE( record->getPreviews().size() == 2 );

				auto previews { record->getPreviewPaths() };
				//Flip array
				std::reverse( previews.begin(), previews.end() );
				REQUIRE_NOTHROW( record->reorderPreviews( previews ) );

				REQUIRE( record->getPreviews().size() == 2 );
				REQUIRE( record->getPreviewPaths() == previews ); //Test that it was flipped
			}

			std::filesystem::remove( "test.png" );
			std::filesystem::remove( "test2.png" );
			std::filesystem::remove_all( "data/images" );
		}

		SECTION( "desc" )
		{
			SECTION( "setDesc" )
			{
				const auto desc { "My description is great!" };
				REQUIRE_NOTHROW( record->setDesc( desc ) );
				SECTION( "getDesc" )
				{
					REQUIRE( record->getDesc() == desc );
				}
			}
		}

		SECTION( "tags" )
		{
			SECTION( "getUserTags" )
			{
				REQUIRE( record->getUserTags().size() == 0 );
			}

			SECTION( "addUserTag" )
			{
				REQUIRE_NOTHROW( record->addUserTag( "test" ) );
				REQUIRE( record->getUserTags().size() == 1 );
				SECTION( "getUserTags" )
				{
					REQUIRE( record->getUserTags()[ 0 ] == "test" );
				}
			}

			SECTION( "getAllTags" )
			{
				record->addUserTag( "test_tag" );

				std::vector< std::string > str {};
				const std::vector< std::string > expected { "creator:test", "engine:test", "test_tag", "title:test" };

				for ( const auto& tag : record->getAllTags() ) str.push_back( tag.toStdString() );

				REQUIRE( str == expected );
			}
		}
	}

	Database::deinit();
}

TEST_CASE( "Database benches", "[!benchmark]" )
{
	std::filesystem::remove( "test.db" );
	Database::initalize( "test.db" );

	const Record record { importRecord( "test", "test", "test" ) };

	int counter { 0 };

	BENCHMARK_ADVANCED( "Add versions" )( Catch::Benchmark::Chronometer meter )
	{
		meter.measure(
			[ & ]
			{
				record->addVersion(
					QString::fromStdString( fmt::format( "v{}.0", counter ) ),
					"C:\\games\\my_game",
					"game.exe",
					1234,
					true );

				++counter;
			} );
	};

	Database::deinit();
	std::filesystem::remove( "test.db" );
}