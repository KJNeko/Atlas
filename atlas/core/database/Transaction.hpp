//
// Created by kj16609 on 6/23/23.
//

#ifndef ATLASGAMEMANAGER_TRANSACTION_HPP
#define ATLASGAMEMANAGER_TRANSACTION_HPP

#include <tracy/Tracy.hpp>

#include "Binder.hpp"

template < bool is_commitable = false >
struct TransactionBase
{
	TransactionBase();

	TransactionBase( const TransactionBase& ) = delete;
	TransactionBase( TransactionBase&& ) = delete;
	TransactionBase& operator=( const TransactionBase& other ) = delete;

	bool m_finished { false };
	std::mutex self_mtx {};
	std::lock_guard< std::mutex > guard;

	Binder operator<<( std::string_view sql )
	{
		if constexpr ( is_commitable )
			sqlite3_exec( &Database::ref(), "BEGIN TRANSACTION;", nullptr, nullptr, nullptr );

		return { sql };
	}

	void commit()
	{
		if constexpr ( is_commitable )
		{
			if ( !m_finished )
			{
				sqlite3_exec( &Database::ref(), "COMMIT TRANSACTION;", nullptr, nullptr, nullptr );
				m_finished = true;
			}
			else
				throw TransactionInvalid( "Attempted to commit a finished transaction" );
		}
	}

	void abort()
	{
		if constexpr ( is_commitable )
		{
			if ( !m_finished )
			{
				sqlite3_exec( &Database::ref(), "ABORT TRANSACTION;", nullptr, nullptr, nullptr );
				m_finished = true;
			}
			else
				throw TransactionInvalid( "Attempted to abort a finished transaction" );
		}
	}

	~TransactionBase() noexcept( false );
};

using Transaction = TransactionBase< true >;
using RapidTransaction = TransactionBase< false >;

#endif //ATLASGAMEMANAGER_TRANSACTION_HPP
