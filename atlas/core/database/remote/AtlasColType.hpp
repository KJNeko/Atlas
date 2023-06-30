//
// Created by kj16609 on 6/30/23.
//

#ifndef ATLASGAMEMANAGER_ATLASCOLTYPE_HPP
#define ATLASGAMEMANAGER_ATLASCOLTYPE_HPP

#include "core/database/Column.hpp"

template <>
struct ColType< "id_name", "atlas_data" >
{
	typedef QString Type;
};

template <>
struct ColType< "title", "atlas_data" >
{
	typedef QString Type;
};

template <>
struct ColType< "creator", "atlas_data" >
{
	typedef QString Type;
};

template <>
struct ColType< "engine", "atlas_data" >
{
	typedef QString Type;
};

#endif //ATLASGAMEMANAGER_ATLASCOLTYPE_HPP
