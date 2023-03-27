//
// Created by kj16609 on 3/26/23.
//

#include <QString>

#include "GTestBox.hpp"
#include "h95/regex.hpp"

TEST( Regex, regexify )
{
	const QString expected {
	"^(?P<creator>[^\\\\/]+)/(?P<title>[^\\\\/]+)/(?P<version>[^\\\\/]+)$"};

	GTEST_ASSERT_EQ(
		expected.toStdString(),
		regexify( "{creator}/{title}/{version}" ).toStdString() );
}

const QString pattern { "/run/media/kj16609/HDDWIN/NSFWSorted/{creator}/{title}/{version}" };

TEST( Regex, patternTest0 )
{
	QString text {
		QString::fromStdString( std::filesystem::
		                            path( "/run/media/kj16609/HDDWIN/NSFWSorted/Acerola/Brave Alchemist Colette/1.04" )
		                                .make_preferred()
		                                .string() )
	};

	GTEST_ASSERT_TRUE( valid( pattern, text ) );

	const auto [ title, creator, version ] = extractGroups( pattern, text );
	GTEST_ASSERT_EQ( title.toStdString(), "Brave Alchemist Colette" );
	GTEST_ASSERT_EQ( creator.toStdString(), "Acerola" );
	GTEST_ASSERT_EQ( version.toStdString(), "1.04" );
}

TEST( Regex, patternTest2 )
{
	QString text {
		QString::fromStdString( std::filesystem::
		                            path( "/run/media/kj16609/HDDWIN/NSFWSorted/ChimeraZak/My Forest Home/v2.35" )
		                                .make_preferred()
		                                .string() )
	};

	GTEST_ASSERT_TRUE( valid( pattern, text ) );
	const auto [ title, creator, version ] = extractGroups( pattern, text );
	GTEST_ASSERT_EQ( title.toStdString(), "My Forest Home" );
	GTEST_ASSERT_EQ( creator.toStdString(), "ChimeraZak" );
	GTEST_ASSERT_EQ( version.toStdString(), "v2.35" );
}

TEST( Regex, patternTest3 )
{
	QString text { QString::fromStdString(
		std::filesystem::path(
			"/run/media/kj16609/HDDWIN/NSFWSorted/dobuworks/Hypnotizing the Rich Bitch into My Personal Plaything/1.1.0" )
			.make_preferred()
			.string() ) };

	GTEST_ASSERT_TRUE( valid( pattern, text ) );
	const auto [ title, creator, version ] = extractGroups( pattern, text );
	GTEST_ASSERT_EQ( title.toStdString(), "Hypnotizing the Rich Bitch into My Personal Plaything" );
	GTEST_ASSERT_EQ( creator.toStdString(), "dobuworks" );
	GTEST_ASSERT_EQ( version.toStdString(), "1.1.0" );
}

TEST( Regex, patternTest10 )
{
	QString text { QString::fromStdString(
		std::filesystem::path( "/run/media/kj16609/HDDWIN/NSFWSorted/Overseer Division Studios/PMC Promiscuity/1.3.2" )
			.make_preferred()
			.string() ) };

	GTEST_ASSERT_TRUE( valid( pattern, text ) );
	const auto [ title, creator, version ] = extractGroups( pattern, text );
	GTEST_ASSERT_EQ( title.toStdString(), "PMC Promiscuity" );
	GTEST_ASSERT_EQ( creator.toStdString(), "Overseer Division Studios" );
	GTEST_ASSERT_EQ( version.toStdString(), "1.3.2" );
}

TEST( Regex, patternTest15 )
{
	QString text { QString::fromStdString(
		std::filesystem::
			path( "/run/media/kj16609/HDDWIN/NSFWSorted/Team-Apple Pie/Monster Black Market/1.2.10.0 - uncensored" )
				.make_preferred()
				.string() ) };

	GTEST_ASSERT_TRUE( valid( pattern, text ) );
	const auto [ title, creator, version ] = extractGroups( pattern, text );
	GTEST_ASSERT_EQ( title.toStdString(), "Monster Black Market" );
	GTEST_ASSERT_EQ( creator.toStdString(), "Team-Apple Pie" );
	GTEST_ASSERT_EQ( version.toStdString(), "1.2.10.0 - uncensored" );
}

TEST( Regex, patternTest16 )
{
	QString text {
		QString::
			fromStdString( std::filesystem::path( "/run/media/kj16609/HDDWIN/NSFWSorted/TsunAmie/Haremon/0.37.5 DEBUG" )
		                       .make_preferred()
		                       .string() )
	};

	GTEST_ASSERT_TRUE( valid( pattern, text ) );
	const auto [ title, creator, version ] = extractGroups( pattern, text );
	GTEST_ASSERT_EQ( title.toStdString(), "Haremon" );
	GTEST_ASSERT_EQ( creator.toStdString(), "TsunAmie" );
	GTEST_ASSERT_EQ( version.toStdString(), "0.37.5 DEBUG" );
}

TEST(Regex, escape)
{
	QString str {"\\ my\\str \\"};
	QString expected {"\\\\ my\\\\str \\\\"};

}


