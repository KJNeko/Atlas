//
// Created by kj16609 on 6/23/23.
//

#include "Transaction.hpp"

namespace internal
{
	static std::atomic< std::thread::id > last_locked {};
}

internal::LockGuardType TransactionData::getLock()
{
	ZoneScoped;
	//Check if we are already locked
	if ( internal::last_locked == std::this_thread::get_id() )
	{
		spdlog::critical( "Deadlock detected! Ejecting!" );
		throw std::runtime_error( "Deadlock" );
	}
	else
		return internal::LockGuardType( Database::lock() );
}

TransactionData::TransactionData() : guard( getLock() )
{
	ZoneScoped;
	internal::last_locked = std::this_thread::get_id();
}

TransactionData::~TransactionData()
{
	ZoneScoped;
	internal::last_locked = std::thread::id();
}

Transaction::Transaction( const TransactionType type ) : data( new TransactionData() ), m_type( type )
{
	ZoneScoped;
	try
	{
		(void)Database::ref();
	}
	catch ( std::runtime_error& e )
	{
		spdlog::error( "Transaction: Database was not ready!" );
		data.reset();
		throw TransactionInvalid( m_previous_statement );
	}

	*this << "BEGIN TRANSACTION";
	data->ran_once = false;
}

Transaction::~Transaction()
{
	ZoneScoped;
	if ( data.use_count() == 1 && !data->invalid )
	{
		if ( m_type == Autocommit )
			commit();
		else
		{
			spdlog::warn( "Transaction defaulted to abort on dtor! Check if this is intended!" );
			abort();
		}
	}
}

void Transaction::commit()
{
	ZoneScoped;
	if ( !data->ran_once )
	{
		spdlog::warn( "commit(): Nothing was done in this Transaction? Check if this is intended" );
	}
	if ( data.use_count() == 0 || data->invalid ) throw TransactionInvalid( m_previous_statement );
	*this << "COMMIT TRANSACTION";

	data->invalid = true;
	releaseData();
}

void Transaction::abort()
{
	ZoneScoped;
	spdlog::warn( "A transaction was aborted! Last executed:\"{}\"", m_previous_statement );
	if ( !data->ran_once )
	{
		spdlog::warn( "abort(): Nothing was done in this Transaction? Check if this is intended" );
	}
	if ( data.use_count() == 0 || data->invalid ) throw TransactionInvalid( m_previous_statement );
	*this << "ROLLBACK TRANSACTION";

	data->invalid = true;
	releaseData();
}

Binder Transaction::operator<<( std::string sql )
{
	ZoneScoped;
	TracyMessage( sql.c_str(), sql.size() );
	if ( data == nullptr ) throw TransactionInvalid( sql );

	data->ran_once = true;
	if ( data.use_count() == 0 ) throw TransactionInvalid( sql );
	m_previous_statement = sql;
	return { std::move( sql ) };
}

Transaction::Transaction( Transaction& other ) : m_parent( &other ), data( other.data ), m_type( other.m_type )
{
	ZoneScoped;
	if ( data == nullptr ) throw TransactionInvalid( other.m_previous_statement );
}
