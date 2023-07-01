//
// Created by kj16609 on 4/22/23.
//

#ifndef ATLASGAMEMANAGER_RECORD_HPP
#define ATLASGAMEMANAGER_RECORD_HPP

#include "RecordData.hpp"

class Record : public std::shared_ptr< RecordData >
{
  public:

	Record() = default;
	Record( const RecordID id );
	Record( Record&& data ) = default;
	Record( const Record& other ) = default;
	Record& operator=( const Record& other ) = default;
};

Q_DECLARE_METATYPE( Record )

//! Imports a record into the database
/**
 *
 * @param title
 * @param creator
 * @param engine
 * @param transaction
 * @throws RecordAlreadyExists
 * @return
 */
Record importRecord( QString title, QString creator, QString engine );

struct RecordException : public std::runtime_error
{
	RecordException( const char* const msg ) : std::runtime_error( msg ) {}
};

struct RecordAlreadyExists : public RecordException
{
	Record record;

	RecordAlreadyExists( Record record_in ) :
	  RecordException( ( "Record already exists with id " + std::to_string( record_in->m_id ) ).c_str() ),
	  record( std::move( record_in ) )
	{}
};

struct InvalidRecordID : public RecordException
{
	RecordID id;

	InvalidRecordID( const RecordID in_id ) :
	  RecordException( ( "Invalid record id = " + std::to_string( in_id ) ).c_str() ),
	  id( in_id )
	{}
};

#endif //ATLASGAMEMANAGER_RECORD_HPP
