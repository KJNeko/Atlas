//
// Created by kj16609 on 9/4/23.
//

#ifndef ATLASGAMEMANAGER_TEMPLATES_HPP
#define ATLASGAMEMANAGER_TEMPLATES_HPP

#include <cstdint>

namespace atlas::database::migrations
{
	//! Up must be defined to create/modify the tables that are required.
	template < std::uint64_t id >
	void up( Transaction& trans );

	void runUp();

} // namespace atlas::database::migrations

/*
#include "core/database/Transaction.hpp"
#include "templates.hpp"

namespace atlas::database::migrations
{
	template <>
	void up< 3 >( Transaction& transaction )
	{
		transaction <<
					R"(

			)";
	}
} // namespace atlas::database::migrations
*/

#endif //ATLASGAMEMANAGER_TEMPLATES_HPP
