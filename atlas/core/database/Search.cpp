//
// Created by kj16609 on 3/10/23.
//

#include "Search.hpp"

#include <moc_Search.cpp>

#include <tracy/Tracy.hpp>

#include "Transaction.hpp"
#include "core/search/QueryBuilder.hpp"

void Search::searchTextChanged( const QString text ) //, const SortOrder order, const bool asc )
{
	ZoneScoped;

	try
	{
		/*
		if ( !text.isEmpty() )
			query = generateQuery( text.toStdString(), order, asc );
		else
			query = "SELECT DISTINCT record_id FROM games NATURAL JOIN last_import_times ORDER BY "
			      + orderToStr( order ) + std::string( asc ? " ASC" : " DESC" );
	  */

		std::vector< atlas::records::Game > records;

		RapidTransaction() << "SELECT DISTINCT record_id FROM games WHERE LOWER(title) LIKE LOWER(?) ORDER BY title"
						   << ( text + "%" )
			>> [ &records ]( const RecordID id )
		{
			if ( id > TESTING_RECORD_ID ) records.emplace_back( id );
		};

		emit searchCompleted( std::move( records ) );
	}
	catch ( std::exception& e )
	{
		atlas::logging::error( fmt::format( "Search failed with query \"{}\": {}", query, e.what() ) );
	}
}
