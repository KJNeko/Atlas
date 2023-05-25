//
// Created by kj16609 on 2/20/23.
//

#ifndef ATLAS_EXECUTEPROC_HPP
#define ATLAS_EXECUTEPROC_HPP

#include <string>

#include <QFuture>

/**
 * @brief Executes the given path
 * @param path
 */
QFuture< int > executeProc( const QString& path );

#endif //ATLAS_EXECUTEPROC_HPP
