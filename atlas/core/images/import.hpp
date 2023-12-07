//
// Created by kj16609 on 9/22/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_IMPORT_HPP
#define ATLASGAMEMANAGER_IMPORT_HPP

#include <QByteArray>
#include <QFuture>
#include <QPixmap>

#include <filesystem>

#include "core/Types.hpp"
#include "core/utils/fileDownloader.hpp"

namespace atlas::images
{
	class Downloader final : public QObject
	{
		Q_OBJECT

		std::unique_ptr< FileDownloader > m_pImgCtrl {};
		RecordID m_game_id { INVALID_RECORD_ID };

	  public:

		QPixmap m_pixmap {};

		void getImage( const QString& path, const RecordID game_id );

	  private slots:
		void imageFinished();

	  signals:
		void finished();
	};

	/**
	 * @param source Source path of the image. Used for logging purposes.
	 * @param byteArray
	 * @param dest
	 */
	void saveImage( const QByteArray& byteArray, const std::filesystem::path& dest );

	[[nodiscard]] std::filesystem::path importImage( const std::filesystem::path& path, const RecordID game_id );

	[[nodiscard]] std::filesystem::path importPixmap( const QPixmap pixmap, const RecordID game_id );

	[[nodiscard]] inline std::filesystem::path importPixmap( const QPixmap pixmap, const atlas::records::Game game )
	{
		return importPixmap( pixmap, game.id() );
	}

	namespace async
	{
		[[nodiscard]] QFuture< std::filesystem::path > importImageFromURL( const QString url, const RecordID game_id );

		//! Stores the image located at `path` in the data folder
		[[nodiscard]] QFuture< std::filesystem::path >
			importImage( const std::filesystem::path& path, const RecordID game_id );

		[[nodiscard]] QFuture< std::filesystem::path > importPixmap( const QPixmap pixmap, const RecordID game_id );

	} // namespace async
} // namespace atlas::images

#endif //ATLASGAMEMANAGER_IMPORT_HPP
