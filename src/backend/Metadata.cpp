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
		case INVALID_RECORD:
			return EmptyMetadata();
			break;
		case GAME_RECORD:
			return GameMetadata::select(id);
			break;
	}

}

Metadata Metadata::insert(const RecordID id, const Metadata& metadata, const RecordType type)
{
	if(auto ptr = std::get_if<EmptyMetadata>(&metadata); ptr) [[unlikely]]
		return metadata;
	else if(auto* g_ptr = std::get_if<GameMetadata>(&metadata); g_ptr)
	{
		GameMetadata::insert( id, std::get< GameMetadata >( metadata ) );
		return metadata;
	}
	else
		return metadata;

	return metadata;
}

