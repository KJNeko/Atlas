//
// Created by kj16609 on 3/26/23.
//

#ifndef HYDRUS95_REGEX_HPP
#define HYDRUS95_REGEX_HPP

#include <QString>

QString groupify( const QString group_name );

//! SHOULD NOT BE USED ANYWHERE EXCEPT FOR PATHS
QString escapeStr( QString pattern );

QString processRegexify( QString pattern );

QString regexify( QString pattern );

bool valid( QString pattern, QString text );

std::tuple< QString, QString, QString, QString > extractGroups( QString pattern, QString text );

#endif //HYDRUS95_REGEX_HPP
