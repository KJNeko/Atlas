//
// Created by kj16609 on 3/10/23.
//

#include "Search.hpp"

#include "core/search/QueryBuilder.hpp"

void Search::searchTextChanged( const QString text, const SortOrder order, const bool asc )
{
	ZoneScoped;
	try
	{
		if ( !text.isEmpty() )
			query = generateQuery( text.toStdString(), order, asc );
		else
			query = "SELECT DISTINCT record_id FROM records NATURAL JOIN last_import_times ORDER BY "
			      + orderToStr( order ) + std::string( asc ? " ASC" : " DESC" );

		runQuery();
	}
	catch ( std::exception& e )
	{
		spdlog::error( "Search failed with query \"{}\": {}", query, e.what() );
	}
}

void Search::runQuery()
{
	if ( query.empty() )
	{
		query = "SELECT DISTINCT record_id FROM records NATURAL JOIN last_import_times ORDER BY "
		      + orderToStr( SortOrder::Name ) + std::string( " ASC" );
	}

	std::vector< Record > records;

	RapidTransaction transaction {};
	transaction << query >> [ & ]( const RecordID id )
	{
		if ( id > 1 ) records.emplace_back( id );
	};

	emit searchCompleted( std::move( records ) );
}