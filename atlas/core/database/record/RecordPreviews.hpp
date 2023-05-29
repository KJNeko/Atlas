//
// Created by kj16609 on 5/24/23.
//

#ifndef ATLASGAMEMANAGER_RECORDPREVIEWS_HPP
#define ATLASGAMEMANAGER_RECORDPREVIEWS_HPP

#include <filesystem>
#include <vector>

#include <QPixmap>

#include "core/database/Database.hpp"

class RecordData;

class RecordPreviews
{
	RecordData& m_record;

	RecordPreviews( RecordData& parent ) : m_record( parent ) {}

	RecordPreviews() = delete;
	RecordPreviews( const RecordPreviews& other ) = delete;
	RecordPreviews( RecordPreviews&& other ) = delete;

	friend class RecordData;

  public:

	const std::vector< std::filesystem::path > getPreviewPaths( Transaction transaction = Transaction( Autocommit ) )
		const;
	std::vector< QPixmap > getPreviews( Transaction transaction = Transaction( Autocommit ) ) const;

	void addPreview( const std::filesystem::path&, Transaction = Transaction( Autocommit ) );
	void removePreview( const std::filesystem::path&, Transaction = Transaction( Autocommit ) );
	void reorderPreviews( const std::vector< std::filesystem::path >& paths, Transaction = Transaction( Autocommit ) );
};

#endif //ATLASGAMEMANAGER_RECORDPREVIEWS_HPP
