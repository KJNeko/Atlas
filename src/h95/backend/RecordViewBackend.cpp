//
// Created by kj16609 on 1/18/23.
//

#include <tracy/Tracy.hpp>
#include "RecordViewBackend.hpp"
#include "h95/database/database.hpp"

std::vector< Record > getAll()
{
	ZoneScoped;
	std::vector< Record > records;

	database::db_ref() << "SELECT record_id FROM records ORDER BY record_id DESC" >> [&]( const RecordID record_id )
	{
		records.emplace_back( Record::select( record_id ) );
	};

	return records;
}

void RecordViewBackend::searchSimilar( [[maybe_unused]] const QString& text )
{
	ZoneScoped;
}

void RecordViewBackend::addTag( [[maybe_unused]] const Tag& tag )
{
	ZoneScoped;
	emit recordsUpdated( getAll() );
}

void RecordViewBackend::removeTag( [[maybe_unused]] const Tag& tag )
{
	ZoneScoped;
	emit recordsUpdated( getAll() );
}

void RecordViewBackend::refresh()
{
	ZoneScoped;
	//TODO: Make true filter eventually
	emit recordsUpdated( getAll() );
}
