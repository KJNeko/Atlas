//
// Created by kj16609 on 6/24/23.
//

#ifndef ATLASGAMEMANAGER_F95DATA_HPP
#define ATLASGAMEMANAGER_F95DATA_HPP

#include <cstdint>

#include <QString>

#include "core/Types.hpp"
#include "core/database/Column.hpp"

struct F95Data
{
  private:

	std::uint64_t f95_id; //Primary key. NOT THE FORM ID

  public:

	template < fgl::string_literal col_name >
	ColType< col_name, "f95_zone_data" >::Type get()
	{
		typename ColType< col_name, "f95_zone_data" >::Type val;
		RapidTransaction() << atlas::database::utility::select_query< col_name, "f95_zone_data", "f95_id" >() << f95_id
			>> val;
		return val;
	}

	template < fgl::string_literal col_name >
	void set( ColType< col_name, "f95_zone_data" >::Type t )
	{
		RapidTransaction() << atlas::database::utility::update_query< col_name, "f95_zone_data", "f95_id" >() << t
						   << f95_id;
	}

	/**
	 * @param id *INTERNAL* Id of the f95 data (Not thread id)
	 */
	F95Data( const F95ID id ) : f95_id( id ) {}
};

#endif //ATLASGAMEMANAGER_F95DATA_HPP
