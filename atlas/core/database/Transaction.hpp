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

	/*
	TransactionBase()
	{
		if constexpr ( is_commitable ) m_finished = false;
	}*/

	TransactionBase( const TransactionBase& ) = delete;
	TransactionBase( TransactionBase&& ) = delete;
	TransactionBase& operator=( const TransactionBase& other ) = delete;

	bool m_finished { false };
	std::mutex self_mtx {};
	std::lock_guard< std::mutex > guard;

	Binder operator<<( std::string sql )
	{
		if constexpr ( is_commitable )
			sqlite3_exec( &Database::ref(), "BEGIN TRANSACTION;", nullptr, nullptr, nullptr );

		return Binder { sql };
	}

	void commit()
	{
		if constexpr ( is_commitable )
		{
			if ( !m_finished )
			{
				sqlite3_exec( &Database::ref(), "COMMIT;", nullptr, nullptr, nullptr );
				Binder( "COMMIT TRANSACTION" );
				m_finished = true;
			}
		}
	}

	~TransactionBase();
	/*{
		if constexpr ( is_commitable )
		{
			if ( !m_finished )
			{
				Binder( "ABORT TRANSACTION" );
				m_finished = true;
			}
		}
	}*/
};

using Transaction = TransactionBase< true >;
using RapidTransaction = TransactionBase< false >;
/*
//! Transaction unit to the database.
struct Transaction
{
	using enum TransactionFlag;

  private:

	Transaction* m_parent { nullptr };

	//! Will be nullptr if m_flags & FastLock
	std::shared_ptr< TransactionData > data;
	TransactionFlag m_flags;
	std::string m_previous_statement {};

	//! Releases the pointer to the shared data section for all Transactions up the chain.
	inline void releaseData()
	{
		data.reset();
		if ( m_parent != nullptr ) m_parent->releaseData();
	}

  public:

	//! @throws TransactionInvalid when trying to create a transaction without the database being initialized first
	Transaction() = delete;

	explicit Transaction( const TransactionFlag type = TransactionFlag::DEFAULT );
	Transaction( Transaction& other );
	Transaction( const Transaction& other ) = delete;
	Transaction( Transaction&& other ) = delete;
	Transaction& operator=( const Transaction& other ) = delete;

	//! @throws TransactionInvalid
	Binder operator<<( std::string sql );

	void commit();


	void abort();

	~Transaction();
};*/
#endif //ATLASGAMEMANAGER_TRANSACTION_HPP
