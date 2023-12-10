//
// Created by kj16609 on 6/23/23.
//

#include "Transaction.hpp"

namespace atlas::database
{

	inline static std::atomic< std::thread::id > last_locked {};
	inline static std::mutex trans_lock {};

	std::lock_guard< std::mutex > getLock()
	{
		if ( last_locked == std::this_thread::get_id() )
		{
			//Deadlock
			throw DatabaseException( "Deadlock detected!" );
		}
		else
		{
			last_locked = std::this_thread::get_id();
			Binder( "BEGIN TRANSACTION" );
			return std::lock_guard( trans_lock );
		}
	}

	template <>
	TransactionBase< true >::TransactionBase() : guard( getLock() )
	{}

	template <>
	TransactionBase< true >::~TransactionBase() noexcept( false )
	{
		if ( !m_finished )
		{
			*this << "ROLLBACK TRANSACTION";
			throw DatabaseException( "Allowed falloff via dtor in TransactionBase<true>!. Rolling back and failing." );
		}

		last_locked = std::thread::id();
	}

	template <>
	void TransactionBase< true >::commit()
	{
		if ( !m_finished )
		{
			//sqlite3_exec( &Database::ref(), "COMMIT TRANSACTION;", nullptr, nullptr, nullptr );
			*this << "COMMIT TRANSACTION";
			m_finished = true;
			atlas::logging::debug( "Commit called" );
		}
		else
			throw TransactionInvalid( "Attempted to commit a finished transaction" );
	}

	template <>
	void TransactionBase< true >::abort()
	{
		if ( !m_finished )
		{
			*this << "ROLLBACK TRANSACTION";
			//sqlite3_exec( &Database::ref(), "ABORT TRANSACTION;", nullptr, nullptr, nullptr );
			m_finished = true;
			atlas::logging::debug( "Abort called" );
		}
		else
			throw TransactionInvalid( "Attempted to abort a finished transaction" );
	}

} // namespace atlas::database