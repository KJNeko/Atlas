//
// Created by kj16609 on 3/26/23.
//

#include <QString>

#include <gtest/gtest.h>
#include "h95/utils/regex/regex.hpp"

TEST( Regex, regexify )
{
	const QString expected { R"(^(?P<creator>[^\\\/]+)/(?P<title>[^\\\/]+)/(?P<version>[^\\\/]+)$)" };

	GTEST_ASSERT_EQ( expected.toStdString(), regexify( "{creator}/{title}/{version}" ).toStdString() );
}

TEST( Regex, WindowsPath )
{
	const QString text { R"(C:\Users\vboxuser\Desktop\Projects\Hydrus95)" };
	const QString pattern { R"(C:\Users\vboxuser\{creator}\{title}\{version})" };

	GTEST_ASSERT_TRUE( valid( pattern, text ) );

	const auto [ title, creator, version, engine ] = extractGroups( regexify( pattern ), text );
	GTEST_ASSERT_EQ( title.toStdString(), "Projects" );
	GTEST_ASSERT_EQ( creator.toStdString(), "Desktop" );
	GTEST_ASSERT_EQ( version.toStdString(), "Hydrus95" );
}

TEST(Regex, TitleVersion)
{
	const QString text { R"(C:\Users\vboxuser\Projects\Hydrus95)" };
	const QString pattern { R"(C:\Users\vboxuser\{title}\{version})" };

	GTEST_ASSERT_TRUE( valid( pattern, text ) );

	const auto [ title, creator, version, engine ] = extractGroups( regexify( pattern ), text );
	GTEST_ASSERT_EQ( title.toStdString(), "Projects" );
	GTEST_ASSERT_EQ( creator.toStdString(), "" );
	GTEST_ASSERT_EQ( version.toStdString(), "Hydrus95" );
}

TEST( Regex, WeirdCharacters )
{
	QString text { "/run/media/kj16609/HDDWIN/NSFWSorted(Copy1)/Acerola/Brave Alchemist Colette/1.04" };

	const QString pattern { "/run/media/kj16609/HDDWIN/NSFWSorted(Copy1)/{creator}/{title}/{version}" };
	GTEST_ASSERT_TRUE( valid( pattern, text ) );

	const auto [ title, creator, version, engine ] = extractGroups( regexify( pattern ), text );
	GTEST_ASSERT_EQ( title.toStdString(), "Brave Alchemist Colette" );
	GTEST_ASSERT_EQ( creator.toStdString(), "Acerola" );
	GTEST_ASSERT_EQ( version.toStdString(), "1.04" );
}

TEST( Regex, WeirdLinuxPath )
{
	QString text { "/run/media/kj16609/HDDWIN/NSFWSorted (Copy 1)/Acerola/Brave Alchemist Colette/1.04" };

	const QString pattern { "/run/media/kj16609/HDDWIN/NSFWSorted (Copy 1)/{creator}/{title}/{version}" };
	GTEST_ASSERT_TRUE( valid( pattern, text ) );

	const auto [ title, creator, version, engine ] = extractGroups( regexify( pattern ), text );
	GTEST_ASSERT_EQ( title.toStdString(), "Brave Alchemist Colette" );
	GTEST_ASSERT_EQ( creator.toStdString(), "Acerola" );
	GTEST_ASSERT_EQ( version.toStdString(), "1.04" );
}

TEST( Regex, SpacesInPath )
{
	QString text { "/run/media/kj16609/HDDWIN/NSFWSorted Copy 1/Acerola/Brave Alchemist Colette/1.04" };

	const QString pattern { "/run/media/kj16609/HDDWIN/NSFWSorted Copy 1/{creator}/{title}/{version}" };
	GTEST_ASSERT_TRUE( valid( pattern, text ) );

	const auto [ title, creator, version, engine ] = extractGroups( regexify( pattern ), text );
	GTEST_ASSERT_EQ( title.toStdString(), "Brave Alchemist Colette" );
	GTEST_ASSERT_EQ( creator.toStdString(), "Acerola" );
	GTEST_ASSERT_EQ( version.toStdString(), "1.04" );
}

TEST( Regex, patternTestLinux0 )
{
	QString text { "/run/media/kj16609/HDDWIN/NSFWSorted/Acerola/Brave Alchemist Colette/1.04" };

	const QString pattern { "/run/media/kj16609/HDDWIN/NSFWSorted/{creator}/{title}/{version}" };
	GTEST_ASSERT_TRUE( valid( pattern, text ) );

	const auto [ title, creator, version, engine ] = extractGroups( regexify( pattern ), text );
	GTEST_ASSERT_EQ( title.toStdString(), "Brave Alchemist Colette" );
	GTEST_ASSERT_EQ( creator.toStdString(), "Acerola" );
	GTEST_ASSERT_EQ( version.toStdString(), "1.04" );
}

TEST( Regex, patternTestLinux1 )
{
	QString text { "/run/media/kj16609/HDDWIN/NSFWSorted/ChimeraZak/My Forest Home/v2.35" };

	const QString pattern { "/run/media/kj16609/HDDWIN/NSFWSorted/{creator}/{title}/{version}" };
	GTEST_ASSERT_TRUE( valid( pattern, text ) );
	const auto [ title, creator, version, engine ] = extractGroups( regexify( pattern ), text );
	GTEST_ASSERT_EQ( title.toStdString(), "My Forest Home" );
	GTEST_ASSERT_EQ( creator.toStdString(), "ChimeraZak" );
	GTEST_ASSERT_EQ( version.toStdString(), "v2.35" );
}

TEST( Regex, patternTestLinux2 )
{
	QString text {
		"/run/media/kj16609/HDDWIN/NSFWSorted/dobuworks/Hypnotizing the Rich Bitch into My Personal Plaything/1.1.0"
	};

	const QString pattern { "/run/media/kj16609/HDDWIN/NSFWSorted/{creator}/{title}/{version}" };
	GTEST_ASSERT_TRUE( valid( pattern, text ) );
	const auto [ title, creator, version, engine ] = extractGroups( regexify( pattern ), text );
	GTEST_ASSERT_EQ( title.toStdString(), "Hypnotizing the Rich Bitch into My Personal Plaything" );
	GTEST_ASSERT_EQ( creator.toStdString(), "dobuworks" );
	GTEST_ASSERT_EQ( version.toStdString(), "1.1.0" );
}

TEST( Regex, patternTestLinux3 )
{
	QString text { "/run/media/kj16609/HDDWIN/NSFWSorted/Overseer Division Studios/PMC Promiscuity/1.3.2" };

	const QString pattern { "/run/media/kj16609/HDDWIN/NSFWSorted/{creator}/{title}/{version}" };
	GTEST_ASSERT_TRUE( valid( pattern, text ) );
	const auto [ title, creator, version, engine ] = extractGroups( regexify( pattern ), text );
	GTEST_ASSERT_EQ( title.toStdString(), "PMC Promiscuity" );
	GTEST_ASSERT_EQ( creator.toStdString(), "Overseer Division Studios" );
	GTEST_ASSERT_EQ( version.toStdString(), "1.3.2" );
}

TEST( Regex, patternTestLinux4 )
{
	QString text { "/run/media/kj16609/HDDWIN/NSFWSorted/Team-Apple Pie/Monster Black Market/1.2.10.0 - uncensored" };

	const QString pattern { "/run/media/kj16609/HDDWIN/NSFWSorted/{creator}/{title}/{version}" };
	GTEST_ASSERT_TRUE( valid( pattern, text ) );
	const auto [ title, creator, version, engine ] = extractGroups( regexify( pattern ), text );
	GTEST_ASSERT_EQ( title.toStdString(), "Monster Black Market" );
	GTEST_ASSERT_EQ( creator.toStdString(), "Team-Apple Pie" );
	GTEST_ASSERT_EQ( version.toStdString(), "1.2.10.0 - uncensored" );
}

TEST( Regex, patternTestLinux5 )
{
	QString text { "/run/media/kj16609/HDDWIN/NSFWSorted/TsunAmie/Haremon/0.37.5 DEBUG" };

	const QString pattern { "/run/media/kj16609/HDDWIN/NSFWSorted/{creator}/{title}/{version}" };
	GTEST_ASSERT_TRUE( valid( pattern, text ) );
	const auto [ title, creator, version, engine ] = extractGroups( regexify( pattern ), text );
	GTEST_ASSERT_EQ( title.toStdString(), "Haremon" );
	GTEST_ASSERT_EQ( creator.toStdString(), "TsunAmie" );
	GTEST_ASSERT_EQ( version.toStdString(), "0.37.5 DEBUG" );
}

TEST( Regex, escape )
{
	QString str { R"(\ my\str \)" };
	QString expected { R"(\\ my\\str \\)" };

	ASSERT_EQ( escapeStr( str ).toStdString(), expected.toStdString() );
}
