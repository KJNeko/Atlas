//
// Created by kj16609 on 3/10/23.
//

#include "Search.hpp"

#include "atlas/search/QueryBuilder.hpp"

void Search::searchTextChanged( [[maybe_unused]] QString text )
{
	if ( text.isEmpty() ) return triggerEmptySearch();

	try
	{
		query = generateQuery( text.toStdString() );
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
		spdlog::error( "Search failed: {}", e.what() );
	}
}

void Search::triggerEmptySearch()
{
	Transaction transaction { Transaction::NoAutocommit };

	std::vector< Record > records;

	transaction << "SELECT record_id FROM records" >> [ & ]( const RecordID id )
	{
		if ( id > 1 ) records.emplace_back( id, transaction );
	};

	transaction.commit();

	emit searchCompleted( std::move( records ) );
}
