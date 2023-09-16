//
// Created by kj16609 on 9/10/23.
//

#ifndef ATLASGAMEMANAGER_RAPIDTRANSACTION_HPP
#define ATLASGAMEMANAGER_RAPIDTRANSACTION_HPP

#include "Transaction.hpp"

namespace atlas::database
{
	using RapidTransaction = TransactionBase< false >;
} // namespace atlas::database

using atlas::database::RapidTransaction;

#endif //ATLASGAMEMANAGER_RAPIDTRANSACTION_HPP
