//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_RECORD_HPP
#define HYDRUS95_RECORD_HPP

#include <QMetaType>
#include "h95/Types.hpp"
#include "GameMetadata.hpp"


struct Record
{
	RecordID m_id;
	QString m_title;
	QString m_creator;
	QString m_engine;

	std::vector< GameMetadata > m_versions;

	std::filesystem::path m_banner;
	std::vector< std::filesystem::path > m_previews;

	private:
	Record() = delete;
	Record(
		const RecordID id,
		const QString title,
		const QString creator,
		const QString engine,
		const std::vector< GameMetadata >& versions,
		const std::filesystem::path& banner,
		const std::vector< std::filesystem::path >& previews ) :
	  m_id( id ),
	  m_title( title ),
	  m_creator( creator ),
	  m_engine( engine ),
	  m_versions( versions ),
	  m_banner( banner ),
	  m_previews( previews )
	{
	}

	public:
	static Record select( const RecordID id );
	static Record create(
		const QString& title,
		const QString& creator,
		const QString& engine,
		const GameMetadata& metadata,
		const std::filesystem::path& banner,
		const std::vector< std::filesystem::path >& previews );

	QPixmap getBanner() const;
	QPixmap getBanner( const int banner_width, const int banner_height ) const;

#ifdef NDEBUG
	bool operator==( const Record& other ) const;
#endif
};

Q_DECLARE_METATYPE( Record )

#endif	//HYDRUS95_RECORD_HPP
