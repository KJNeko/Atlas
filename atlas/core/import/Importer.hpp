//
// Created by kj16609 on 6/2/23.
//

#ifndef ATLASGAMEMANAGER_IMPORTER_HPP
#define ATLASGAMEMANAGER_IMPORTER_HPP

#include <QFuture>
#include <QObject>
#include <QPromise>

#include <filesystem>

#include "core/Types.hpp"
#include "core/database/record/game/Game.hpp"

struct GameImportData;

/**
 *
 * @param data
 * @param root Should be populated with the folder containing the `root` for the `data` input. Leave empty if the `data` input is already a full path.
 * @param owning see `importGame`
 * @return
 */
[[nodiscard]] QFuture< RecordID >
	importGame( GameImportData data, const std::filesystem::path root, const bool owning );

#endif //ATLASGAMEMANAGER_IMPORTER_HPP
