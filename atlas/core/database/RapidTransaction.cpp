//
// Created by kj16609 on 9/10/23.
//

#include "RapidTransaction.hpp"

namespace atlas::database
{
	template <>
	TransactionBase< false >::TransactionBase() : guard( self_mtx )
	{}

	template <>
	TransactionBase< false >::~TransactionBase() noexcept( false )
	{}

	template <>
	void TransactionBase< false >::commit()
	{
		throw TransactionInvalid( "Attempted to commit a non-transaction" );
	}

	template <>
	void TransactionBase< false >::abort()
	{
		throw TransactionInvalid( "Attempted to abort a non-transaction" );
	}

} // namespace atlas::database