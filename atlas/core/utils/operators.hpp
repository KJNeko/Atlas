//
// Created by kj16609 on 8/31/23.
//

#ifndef ATLASGAMEMANAGER_OPERATORS_HPP
#define ATLASGAMEMANAGER_OPERATORS_HPP

#include <QString>

#include <filesystem>

#ifdef __linux__
inline std::filesystem::path operator/( const std::filesystem::path path, const QString second )
{
	return path / second.toStdString();
}
#else
inline std::filesystem::path operator/( const std::filesystem::path path, const QString second )
{
	return path / second.toStdWString();
}
#endif

#endif //ATLASGAMEMANAGER_OPERATORS_HPP
