//
// Created by kj16609 on 1/18/23.
//

#include "RecordViewBackend.hpp"
#include "h95/database/database.hpp"

std::vector< Record > getAll()
{
	std::vector< Record > records;

	database::db_ref() << "SELECT record_id FROM records ORDER BY record_id DESC" >> [&]( const RecordID record_id )
	{
		records.emplace_back( Record::select( record_id ) );
	};

	return records;
}

void RecordViewBackend::searchSimilar( const QString& text ) {}

void RecordViewBackend::addTag( const Tag& tag )
{
	emit recordsUpdated( getAll() );
}

void RecordViewBackend::removeTag( const Tag& tag )
{
	emit recordsUpdated( getAll() );
}

void RecordViewBackend::refresh()
{
	//TODO: Make true filter eventually
	emit recordsUpdated( getAll() );
}
