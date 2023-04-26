//
// Created by kj16609 on 4/22/23.
//

#include "Record.hpp"

namespace internal
{
	inline static std::unordered_map< RecordID, std::shared_ptr< RecordData > > map;
	inline static std::mutex map_mtx;

	//! Returns the ptr for the given key. Returns nullptr if not found
	inline static std::shared_ptr< RecordData > getPtr( const RecordID key, Transaction& transaction )
	{
		std::lock_guard guard { map_mtx };
		if ( auto itter = map.find( key ); itter != map.end() )
			return itter->second;
		else
		{
			auto ptr = std::make_shared< RecordData >( key, transaction );
			map.emplace( key, ptr );
			return ptr;
		}
	}

	inline static std::shared_ptr< RecordData > movePtr( RecordData&& data )
	{
		std::lock_guard guard { map_mtx };

		if ( auto itter = map.find( data.getID() ); itter != map.end() )
			return itter->second;
		else
		{
			auto ptr = std::make_shared< RecordData >( std::move( data ) );
			map.emplace( ptr->getID(), ptr );
			return ptr;
		}
	}

} // namespace internal

Record::Record( const RecordID id, Transaction transaction ) :
  std::shared_ptr< RecordData >( internal::getPtr( id, transaction ) )
{}

Record::Record( RecordData&& data ) :
  std::shared_ptr< RecordData >( internal::movePtr( std::forward< RecordData >( data ) ) )
{}

Record::Record( const RecordData& data, Transaction transaction ) :
  std::shared_ptr< RecordData >( internal::getPtr( data.getID(), transaction ) )
{}

Record::Record( QString title, QString creator, QString engine, Transaction transaction ) :
  Record( RecordData( std::move( title ), std::move( creator ), std::move( engine ), transaction ), transaction )
{}

//! imports a new record and returns it. Will return an existing record if the record already exists
Record importRecord( QString title, QString creator, QString engine, Transaction transaction )
{
	if ( recordExists( title, creator, engine, transaction ) )
		throw RecordAlreadyExists( Record(
			recordID( std::move( title ), std::move( creator ), std::move( engine ), transaction ), transaction ) );

	try
	{
		return Record( std::move( title ), std::move( creator ), std::move( engine ), transaction );
	}
	catch ( std::exception& e )
	{
		spdlog::error( "Failed to import record: {}", e.what() );
		std::rethrow_exception( std::current_exception() );
	}
	catch ( ... )
	{
		spdlog::error( "Failed to import record: Unknown error" );
		std::rethrow_exception( std::current_exception() );
	}
}
