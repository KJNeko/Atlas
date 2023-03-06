//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_RECORD_HPP
#define HYDRUS95_RECORD_HPP

#include <h95/Types.hpp>
#include <h95/database/GameMetadata.hpp>

#include <QPixmap>

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

	public:
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

	//! Selects a record from the database.
	static Record select( const RecordID id, Transaction& transaction );


	//! Searches for the record and returns it's id. 0 if failed.
	/**
	 *
	 * @param title
	 * @param creator
	 * @param engine
	 * @param transaction
	 * @return
	 */
	static RecordID
		search( const QString& title, const QString& creator, const QString& engine, Transaction& transaction );

	//! Updates a record with new information.
	static void update( const RecordID id, Record& record, Transaction& transaction );

	static void erase( const RecordID, Transaction& transaction );


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
		const std::vector< GameMetadata >& metadata,
		const std::filesystem::path& banner,
		const std::vector< std::filesystem::path >& previews,
		Transaction& transaction );

	//! Returns the banner for the record
	/**
	 * @note Returns ":/invalid_banner.jpg" if banner could not be found
	 * @return
	 */
	QPixmap getBanner() const;

	//! Returns a resized banner for the given record
	/**
	 *
	 * @param banner_width
	 * @param banner_height
	 * @return
	 * @note Returns ":/invalid_banner.jpg" if banner could not be found
	 */
	QPixmap getBanner( const int banner_width, const int banner_height ) const;

	bool operator==( const Record& other ) const = default;

	friend class TestRecord;
};

struct RecordException : public std::runtime_error
{
	RecordException( const char* const msg ) : std::runtime_error( msg ) {}
};

struct RecordAlreadyExists : public RecordException
{
	Record record;
	RecordAlreadyExists( Record record_in ) :
	  RecordException( ( "Record already exists with id " + std::to_string( record_in.m_id ) ).c_str() ),
	  record( std::move( record_in ) )
	{
	}
};

struct InvalidRecordID : public RecordException
{
	RecordID id;
	InvalidRecordID( const RecordID in_id ) :
	  RecordException( ( "Invalid record id = " + std::to_string( in_id ) ).c_str() ),
	  id( in_id )
	{
	}
};

#endif	//HYDRUS95_RECORD_HPP
