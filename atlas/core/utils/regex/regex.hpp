//
// Created by kj16609 on 3/26/23.
//

#pragma once
#ifndef ATLAS_REGEX_HPP
#define ATLAS_REGEX_HPP

#include <QString>

#if ( QT_VERSION < QT_VERSION_CHECK( 6, 1, 0 ) )
#error "Atlas requires Qt 6.1.0 or newer"
#endif

namespace regex
{
	QString groupify( const QString group_name );

	//! SHOULD NOT BE USED ANYWHERE EXCEPT FOR PATHS
	QString escapeStr( QString pattern );

	QString processRegexify( QString pattern );

	QString regexify( QString pattern );

	bool valid( QRegularExpression& regex, QString text );
	bool valid( QString pattern, QString text );

	struct GroupsOutput
	{
		QString title;
		QString creator;
		QString version;
		QString engine;
	};

	//! Extracts all groups from a given string with a given pattern.
	GroupsOutput extractGroups( QString pattern, QString text );
} // namespace regex
#endif //ATLAS_REGEX_HPP
