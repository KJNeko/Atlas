//
// Created by kj16609 on 6/24/23.
//

#ifndef ATLASGAMEMANAGER_F95DATA_HPP
#define ATLASGAMEMANAGER_F95DATA_HPP

#include <cstdint>

#include <QString>

#include "F95ColType.hpp"
#include "core/Types.hpp"

struct F95Data
{
  private:

	std::uint64_t f95_id; //Primary key. NOT THE FORM ID

  public:

	template < F95Columns col >
	F95ColType< col > get()
	{
		F95ColType< col > val;
		RapidTransaction()
				<< atlas::database::utility::select_query< F95ColInfo< col >::col_name, "f95_zone_data", "f95_id" >()
				<< f95_id
			>> val;
		return val;
	}

	template < F95Columns col >
	void set( F95ColType< col > t )
	{
		RapidTransaction()
			<< atlas::database::utility::update_query< F95ColInfo< col >::col_name, "f95_zone_data", "f95_id" >() << t
			<< f95_id;
	}

	/**
	 * @param id *INTERNAL* Id of the f95 data (Not thread id)
	 */
	F95Data( const F95ID id ) : f95_id( id ) {}
};

#endif //ATLASGAMEMANAGER_F95DATA_HPP
