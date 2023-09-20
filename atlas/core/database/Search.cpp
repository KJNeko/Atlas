//
// Created by kj16609 on 3/10/23.
//

#include "Search.hpp"

#include <moc_Search.cpp>

#include <tracy/Tracy.hpp>

#include "RapidTransaction.hpp"

void Search::searchTextChanged( const QString& text )
try //, const SortOrder order, const bool asc )
{
	ZoneScoped;

	std::vector< atlas::records::Game > records;

	RapidTransaction() << "SELECT DISTINCT record_id FROM games WHERE LOWER(title) LIKE LOWER(?) ORDER BY title"
					   << ( "%" + text + "%" )
		>> [ &records ]( const RecordID id )
	{
		if ( id > TESTING_RECORD_ID ) records.emplace_back( id );
	};

	emit searchCompleted( std::move( records ) );
}
catch ( std::exception& e )
{
	atlas::logging::error( "{}", e.what() );
}
