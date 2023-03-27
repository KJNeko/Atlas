//
// Created by kj16609 on 3/26/23.
//

#ifndef HYDRUS95_REGEX_HPP
#define HYDRUS95_REGEX_HPP

#include <QDir>
#include <QRegularExpression>
#include <QString>

#include <tracy/Tracy.hpp>

#include "h95/logging.hpp"

inline QString groupify( const QString group_name )
{
	return "(?P<" + group_name.mid( 1, group_name.size() - 2 ) + ">[^\\\\\\/]+)";
}

//! SHOULD NOT BE USED ANYWHERE EXCEPT FOR PATHS
inline QString escapeStr( QString pattern )
{
	for ( int i = 0; i < pattern.size(); ++i )
	{
		if ( pattern.at( i ) == '\\' )
		{
			if ( i + 1 >= pattern.size() )
				continue;
			else if ( pattern.at( i + 1 ) == '\\' || pattern.at( i + 1 ) == '/' )
				//Skip the next character since we know it's escaped.
			{
				i += 1;
				continue;
			}
			else
			{
				pattern.insert( i, '\\' );
				i += 1;
				continue;
			}
		}
	}
	return pattern;
}

inline QString regexify( QString pattern )
{
	ZoneScoped;
	pattern = escapeStr( std::move( pattern ) );

	if ( pattern.contains( QRegularExpression( "{.*?}" ) ) )
	{
		//We found a complete set.
		const auto start { pattern.indexOf( '{' ) };
		const auto end { pattern.indexOf( '}', start ) };

		const auto extract { pattern.mid( start, ( end - start ) + 1 ) };
		pattern.replace( extract, groupify( extract ) );
		return regexify( std::move( pattern ) );
	}
	else
		return "^" + std::move( pattern ) + "$";
}

bool valid( QString pattern, QString text )
{
	ZoneScoped;
	if ( pattern.contains( '{' ) && pattern.contains( '}' ) ) pattern = regexify( std::move( pattern ) );
	QRegularExpression regex { pattern };
	const auto match { regex.match( text ) };
	return match.hasMatch();
}

std::tuple< QString, QString, QString > extractGroups( QString pattern, QString text )
{
	ZoneScoped;
	if ( pattern.contains( '{' ) && pattern.contains( '}' ) ) pattern = regexify( std::move( pattern ) );
	const QRegularExpression regex { pattern };
	const auto match { regex.match( text ) };

	if ( match.hasMatch() && match.hasCaptured( "title" ) )
	{
		auto title { match.captured( "title" ) };
		auto creator { match.hasCaptured( "creator" ) ? match.captured( "creator" ) : "" };
		auto version { match.hasCaptured( "version" ) ? match.captured( "version" ) : "" };

		return { std::move( title ), std::move( creator ), std::move( version ) };
	}
	else
		throw std::runtime_error( "Missing required title capture" );
}

#endif //HYDRUS95_REGEX_HPP
