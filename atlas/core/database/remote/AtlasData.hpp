//
// Created by kj16609 on 6/24/23.
//

#ifndef ATLASGAMEMANAGER_ATLASDTA_HPP
#define ATLASGAMEMANAGER_ATLASDTA_HPP

#include <cstdint>

#include <QString>

#include "AtlasColType.hpp"
#include "core/Types.hpp"
#include "core/database/Transaction.hpp"
#include "core/fgl/string_literal.hpp"

//Before: 2269 bytes
//After: 1124 bytes
struct AtlasData
{
	AtlasID atlas_id;

  private:

	static constexpr fgl::string_literal table_name { "atlas_data" };
	static constexpr fgl::string_literal key_name { "atlas_id" };

  public:

	template < fgl::string_literal col_name >
	ColType< col_name, "atlas_data" >::Type get()
	{
		typename ColType< col_name, "atlas_data" >::Type val;
		RapidTransaction() << atlas::database::utility::select_query< col_name, "atlas_data", "atlas_id" >() << atlas_id
			>> val;
		return val;
	}

	template < fgl::string_literal col_name >
	void set( ColType< col_name, "atlas_data" >::Type t )
	{
		RapidTransaction() << atlas::database::utility::update_query< col_name, "atlas_data", "atlas_id" >() << t
						   << atlas_id;
	}

	AtlasData( const AtlasID id ) : atlas_id( id ) {}
};

#endif //ATLASGAMEMANAGER_ATLASDTA_HPP
