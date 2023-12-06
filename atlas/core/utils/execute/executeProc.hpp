//
// Created by kj16609 on 2/20/23.
//

#pragma once
#ifndef ATLAS_EXECUTEPROC_HPP
#define ATLAS_EXECUTEPROC_HPP

#include <QFuture>

#include <string>

#include "core/Types.hpp"

/**
 * @brief Executes the given path
 * @param path
 */
void executeProc( const RecordID game_id, const QString version, const QString& path );
bool processIsRunning();
void softTerminateProcess();
void hardTerminateProcess();

#endif //ATLAS_EXECUTEPROC_HPP
