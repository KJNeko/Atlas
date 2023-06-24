//
// Created by kj16609 on 4/22/23.
//

#include "Record.hpp"

#include <tracy/Tracy.hpp>

namespace internal
{
	inline static std::unordered_map< RecordID, std::shared_ptr< RecordData > > map;
	inline static std::mutex map_mtx;

	//! Returns the ptr for the given key. Returns nullptr if not found
	inline static std::shared_ptr< RecordData > getPtr( const RecordID key )
	{
		ZoneScoped;
		std::lock_guard guard { map_mtx };
		if ( auto itter = map.find( key ); itter != map.end() )
			return itter->second;
		else
		{
			auto ptr = std::make_shared< RecordData >( key );
			map.emplace( key, ptr );
			return ptr;
		}
	}

	inline static std::shared_ptr< RecordData > movePtr( RecordData&& data )
	{
		ZoneScoped;
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

Record::Record( const RecordID id ) : std::shared_ptr< RecordData >( internal::getPtr( id ) )
{}

Record::Record( RecordData&& data ) :
  std::shared_ptr< RecordData >( internal::movePtr( std::forward< RecordData >( data ) ) )
{}

//! imports a new record and returns it. Will return an existing record if the record already exists
Record importRecord( QString title, QString creator, QString engine )
{
	ZoneScoped;
	if ( recordExists( title, creator, engine ) )
		throw RecordAlreadyExists(
			Record( recordID( std::move( title ), std::move( creator ), std::move( engine ) ) ) );

	RecordData data { std::move( title ), std::move( creator ), std::move( engine ) };

	return Record( data.getID() );
}
