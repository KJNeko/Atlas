//
// Created by kj16609 on 6/23/23.
//

#include "Transaction.hpp"

inline static std::atomic< std::thread::id > last_locked {};
inline static std::mutex trans_lock;

std::lock_guard< std::mutex > getLock()
{
	if ( last_locked == std::this_thread::get_id() )
	{
		//Deadlock
		throw std::runtime_error( "Deadlock detected!" );
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
TransactionBase< true >::~TransactionBase()
{
	if ( !m_finished )
	{
		Binder( "ROLLBACK TRANSACTION" );
		throw std::runtime_error( "Allowed falloff via dtor in TransactionBase<true>!. Rolling back and failing." );
	}

	last_locked = std::thread::id( 0 );
}

template <>
TransactionBase< false >::TransactionBase() : guard( self_mtx )
{}

template <>
TransactionBase< false >::~TransactionBase()
{}