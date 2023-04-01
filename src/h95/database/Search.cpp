//
// Created by kj16609 on 3/10/23.
//

#include "Search.hpp"

void Search::searchTextChanged( [[maybe_unused]] QString text )
{
	//
}

void Search::addTag( [[maybe_unused]] QString tag_text )
{
	emit searchCompleted( search() );
}

void Search::removeTag( [[maybe_unused]] QString tag_text )
{
	emit searchCompleted( search() );
}

const std::vector< QString >& Search::getTagsActive() const
{
	return active_tags;
}

std::vector< Record > Search::search()
{
	Transaction transaction { Transaction::NoAutocommit };

	std::vector< Record > records;

	//TODO: Actual implement the search. For now this just returns ALL records.
	transaction << "SELECT record_id FROM records" >> [ & ]( const RecordID id )
	{ records.emplace_back( id, transaction ); };

	transaction.commit();

	return records;
}

void Search::triggerSearch()
{
	emit searchCompleted( search() );
}
