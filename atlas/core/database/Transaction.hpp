//
// Created by kj16609 on 6/23/23.
//

#ifndef ATLASGAMEMANAGER_TRANSACTION_HPP
#define ATLASGAMEMANAGER_TRANSACTION_HPP

#include <tracy/Tracy.hpp>

#include <exception>
#include <mutex>
#include <string>
#include <string_view>

#include "Binder.hpp"
#include "Database.hpp"
#include "core/logging.hpp"

namespace atlas::database
{
	struct TransactionInvalid : public std::runtime_error
	{
		TransactionInvalid( std::string m_sql_string ) :
		  std::runtime_error( fmt::format( "Transaction accessed while invalid: Last executed: {}", m_sql_string ) )
		{}
	};

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

		template < std::uint64_t size >
		Binder operator<<( const char ( &raw_str )[ size - 1 ] )
		{
			const std::string_view str_view { std::string_view( raw_str, size - 1 ) };
			return *this << str_view;
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

} // namespace atlas::database

using Transaction = atlas::database::TransactionBase< true >;
using RapidTransaction = atlas::database::TransactionBase< false >;

#endif //ATLASGAMEMANAGER_TRANSACTION_HPP
