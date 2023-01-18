//
// Created by kj16609 on 1/16/23.
//

#include "Metadata.hpp"
#include "Record.hpp"
#include <QSqlQuery>

Metadata Metadata::select(const RecordID id, const RecordType type)
{
	switch(type)
	{
		default: [[fallthrough]];
		case INVALID_RECORD:
			return {EmptyMetadata()};
			break;
		case GAME_RECORD:
			return {GameMetadata::select(id)};
			break;
	}

}

Metadata Metadata::insert(const RecordID id, const Metadata& metadata, const RecordType type)
{
	switch(type)
	{
		default: [[fallthrough]];
		case INVALID_RECORD:
			return {EmptyMetadata()};
		case GAME_RECORD:
			return {GameMetadata::insert(id, std::get<GameMetadata>(metadata))};
	}
}

