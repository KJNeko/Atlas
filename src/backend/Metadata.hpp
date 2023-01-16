//
// Created by kj16609 on 1/15/23.
//

#ifndef HYDRUS95_METADATA_HPP
#define HYDRUS95_METADATA_HPP

#include "GameMetadata.hpp"
//#include "AssetMetadata.hpp"

enum RecordType : int
{
	INVALID_RECORD = 0,
	GAME_RECORD
};

struct EmptyMetadata
{
	bool operator==(const EmptyMetadata& other) const {return true;}
};

struct Metadata : public std::variant<EmptyMetadata, GameMetadata>
{
	static Metadata select(const RecordID id, const RecordType type);
	static Metadata insert(const RecordID id, const Metadata& metadata, const RecordType type);
};

#endif	//HYDRUS95_METADATA_HPP
