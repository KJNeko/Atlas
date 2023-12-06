//
// Created by kj16609 on 9/4/23.
//

#pragma once
#ifndef ATLASGAMEMANAGER_TEMPLATES_HPP
#define ATLASGAMEMANAGER_TEMPLATES_HPP

#include <cstdint>
#include <string_view>

namespace atlas::database::migrations
{
	//! Up must be defined to create/modify the tables that are required.
	/**
	 * @tparam id ID of the migration to run
	 * @return Returns the migration string. Must be a valid SQL query
	 */
	template < std::uint64_t id >
	[[nodiscard]] std::string_view migration();

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
