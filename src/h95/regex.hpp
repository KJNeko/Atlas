//
// Created by kj16609 on 3/26/23.
//

#ifndef HYDRUS95_REGEX_HPP
#define HYDRUS95_REGEX_HPP

#include <QDir>
#include <QRegularExpression>
#include <QString>

#include "h95/logging.hpp"

inline QString groupify( const QString group_name )
{
	return "(?P<" + group_name.mid( 1, group_name.size() - 2 ) + ">[^\\\\/]+)";
}

inline QString cleanPathDelim( QString path )
{
	return QString::fromStdString( std::filesystem::path( path.toStdString() ).make_preferred().string() );
}

inline QString regexify( QString pattern )
{
	ZoneScoped;
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
		return cleanPathDelim( "^" + std::move( pattern ) + "$" );
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
