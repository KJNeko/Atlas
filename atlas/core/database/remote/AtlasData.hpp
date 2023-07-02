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

	template < AtlasColumns col >
	AtlasColType< col > get()
	{
		AtlasColType< col > val {};
		RapidTransaction()
				<< atlas::database::utility::select_query< AtlasColInfo< col >::col_name, "atlas_data", "atlas_id" >()
				<< atlas_id
			>> val;
		return val;
	}

	template < AtlasColumns... cols >
		requires( sizeof...( cols ) > 1 )
	std::tuple< AtlasColType< cols >... > get()
	{
		std::tuple< AtlasColType< cols >... > tpl {};
		RapidTransaction() << atlas::database::utility::
					select_query_t< "atlas_data", "atlas_id", AtlasColInfo< cols >::col_name... >()
						   << atlas_id
			>> tpl;
		return tpl;
	}

	template < AtlasColumns col >
	void set( AtlasColType< col > t )
	{
		RapidTransaction()
			<< atlas::database::utility::update_query< AtlasColInfo< col >::col_name, "atlas_data", "atlas_id" >() << t
			<< atlas_id;
	}

	AtlasData( const AtlasID id ) : atlas_id( id ) {}
};

#endif //ATLASGAMEMANAGER_ATLASDTA_HPP
