//
// Created by towerwatchman on 01/24/24.
//

#pragma once
#ifndef ATLAS_EXTRACTOR_HPP
#define ATLAS_EXTRACTOR_HPP

#include "core/import/GameImportData.hpp"
#include "ui/notifications/ProgressNotification.hpp"

namespace atlas::utils
{
	class Extractor final : public QObject
	{
		Q_OBJECT
		Q_DISABLE_COPY_MOVE( Extractor )
		std::unique_ptr< ProgressSignaler > m_signaler { nullptr };
		//! Returns the byte size of a folder

	  public:

		Extractor() = default;
		bool extractArchive( GameImportData game );

		void extractionProgress( qint64 bytesReceived, qint64 bytesTotal );

		void fileProgress( std::string file );

	}; // namespace atlas::extractor
} // namespace atlas::utils

#endif //ATLAS_EXTRACTOR_HPP