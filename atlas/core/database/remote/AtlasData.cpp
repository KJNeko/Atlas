//
// Created by kj16609 on 6/28/23.
//

#include "AtlasData.hpp"

template <>
struct ColType< "id_name", "atlas_data" >
{
	typedef QString Type;
};
