//
// Created by kj16609 on 6/25/23.
//

#include "RecordData.hpp"
#include "core/database/remote/AtlasData.hpp"
#include "core/database/remote/F95Data.hpp"

F95Data RecordData::f95Data()
{
	F95ID id { 0 };
	RapidTransaction() << "SELECT f95_id FROM f95_mappings WHERE record_id = ?" << m_id >> id;
	return F95Data { id };
}

AtlasData RecordData::atlasData()
{
	AtlasID id { 0 };
	RapidTransaction() << "SELECT atlas_id FROM atlas_mappings WHERE record_id = ?" << m_id >> id;
	return AtlasData { id };
}

void RecordData::linkAtlasData( const AtlasID id )
{
	RapidTransaction() << "INSERT INTO f95_mappings (record_id, f95_id) VALUES (?, ?)" << m_id << id;
}
