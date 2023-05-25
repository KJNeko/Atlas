//
// Created by kj16609 on 3/10/23.
//

#include "Search.hpp"

#include "core/search/QueryBuilder.hpp"

void Search::searchTextChanged( [[maybe_unused]] QString text, const SortOrder order, const bool asc )
{
	try
	{
		if ( !text.isEmpty() )
			query = generateQuery( text.toStdString(), order, asc );
		else
			query = "SELECT DISTINCT record_id FROM records NATURAL JOIN last_import_times ORDER BY "
			      + orderToStr( order ) + std::string( asc ? " ASC" : " DESC" );
		Transaction transaction { Transaction::NoAutocommit };

		std::vector< Record > records;

		transaction << query >> [ & ]( const RecordID id )
		{
			if ( id > 1 ) records.emplace_back( id, transaction );
		};

		transaction.commit();

		emit searchCompleted( std::move( records ) );
	}
	catch ( std::exception& e )
	{
		spdlog::error( "Search failed with query \"{}\": {}", query, e.what() );
	}
}