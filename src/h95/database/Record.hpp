//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_RECORD_HPP
#define HYDRUS95_RECORD_HPP

#include <QMetaType>
#include "h95/Types.hpp"
#include "GameMetadata.hpp"

//! Thrown when attempting to create a record that already exists
struct RecordAlreadyExists : public std::runtime_error
{
	RecordID m_id;

	RecordAlreadyExists( const char* const msg, const RecordID id ) : std::runtime_error( msg ), m_id( id ) {}
};

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

	//! Internal, used in select and create
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
	//! Selects a record from the database.
	static Record select( const RecordID id );

	//! Updates a record with new information.
	static Record update( const RecordID id, const Record& record );

	/**
	 * @param title unique constraint
	 * @param creator unique constraint
	 * @param engine unique constraint
	 * @param metadata
	 * @param banner
	 * @param previews
	 * @return
	 * @throws RecordAlreadyExists
	 */
	//! Creates a record and inserts it into the database.
	static Record create(
		const QString& title,
		const QString& creator,
		const QString& engine,
		const GameMetadata& metadata,
		const std::filesystem::path& banner,
		const std::vector< std::filesystem::path >& previews );

	//! Returns the banner for the record
	QPixmap getBanner() const;

	//! Returns a resized banner for the given record
	QPixmap getBanner( const int banner_width, const int banner_height ) const;

#ifdef NDEBUG
	//! operator== override for faster comparisons during release builds (only compares RecordID)
	bool operator==( const Record& other ) const;
#endif
};

Q_DECLARE_METATYPE( Record )

#endif	//HYDRUS95_RECORD_HPP
