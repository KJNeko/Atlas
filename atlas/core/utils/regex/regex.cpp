//
// Created by kj16609 on 3/31/23.
//

#include "regex.hpp"

#include <QRegularExpression>
#include <QString>

#include <tracy/Tracy.hpp>

#include "core/logging.hpp"

namespace regex
{
	QString groupify( const QString group_name )
	{
		ZoneScoped;
		return R"((?P<)" + group_name.mid( 1, group_name.size() - 2 ) + R"(>[^\\\/]+))";
	}

	//! SHOULD NOT BE USED ANYWHERE EXCEPT FOR PATHS
	QString escapeStr( QString pattern )
	{
		ZoneScoped;
		const QRegularExpression regex { R"((?<!\\)[\(\)]|(?<!\\)[\\](?![\/\\\(\)\?$]))" };

		while ( pattern.contains( regex ) )
		{
			const auto index { pattern.indexOf( regex ) };
			const auto character { pattern.at( index ) };
			pattern.replace( index, 1, QString( "\\" ) + character );
		}

		return pattern;
	}

	QString processRegexify( QString pattern )
	{
		ZoneScoped;
		if ( pattern.contains( QRegularExpression( "{.*?}" ) ) )
		{
			//We found a complete set.
			const auto start { pattern.indexOf( '{' ) };
			const auto end { pattern.indexOf( '}', start ) };

			const auto extract { pattern.mid( start, ( end - start ) + 1 ) };
			pattern.replace( extract, groupify( extract ) );
			return processRegexify( std::move( pattern ) );
		}
		else
			return std::move( pattern ) + "$";
	}

	QString regexify( QString pattern )
	{
		ZoneScoped;
		return processRegexify( escapeStr( std::move( pattern ) ) );
	}

	bool valid( QRegularExpression& regex, QString text )
	{
		const auto match { regex.match( text ) };
		return match.hasMatch();
	}

	bool valid( QString pattern, QString text )
	{
		ZoneScoped;
		if ( pattern.contains( '{' ) && pattern.contains( '}' ) ) pattern = regexify( std::move( pattern ) );
		const QRegularExpression regex { pattern };
		const auto match { regex.match( text ) };
		return match.hasMatch();
	}

	//std::tuple< QString, QString, QString, QString >
	GroupsOutput extractGroups( QString pattern, const QString text )
	{
		ZoneScoped;
		if ( pattern.contains( '{' ) && pattern.contains( '}' ) ) pattern = regexify( std::move( pattern ) );
		const QRegularExpression regex { pattern };
		const auto match { regex.match( text ) };

		auto title { match.captured( "title" ) };
//		auto creator { match.hasCaptured( "creator" ) ? match.captured( "creator" ) : "" };
//		auto version { match.hasCaptured( "version" ) ? match.captured( "version" ) : "" };
//		auto engine { match.hasCaptured( "engine" ) ? match.captured( "engine" ) : "" };
		auto creator { match.captured( "creator" )};
		auto version { match.captured( "version" )};
		auto engine { match.captured( "engine" )};

		return { std::move( title ), std::move( creator ), std::move( version ), std::move( engine ) };
	}
} // namespace regex