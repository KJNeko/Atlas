//
// Created by kj16609 on 1/12/23.
//

#ifndef ATLAS_DATABASE_HPP
#define ATLAS_DATABASE_HPP

#include <filesystem>
#include <sqlite3.h>

#include <QObject>

#include "core/logging.hpp"

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wundef"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wextra-semi"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#pragma GCC diagnostic ignored "-Wpragmas"

#include <tracy/Tracy.hpp>

#pragma GCC diagnostic pop

namespace internal
{
#ifdef TRACY_ENABLE
	using MtxType = tracy::Lockable< std::mutex >;
	using LockGuardType = std::lock_guard< tracy::Lockable< std::mutex > >;
#else
	using MtxType = std::mutex;
	using LockGuardType = std::lock_guard< std::mutex >;
#endif
} // namespace internal

class Database
{
	//! Returns a ref to the sqlite DB
	static sqlite3& ref();

	//! Returns a ref to the global DB lock
	static internal::MtxType& lock();

  public:

	//! Initalizes the database with init_path. Does not have to be caonical
	static void initalize( const std::filesystem::path init_path );
	//! Deinitalizes the DB.
	static void deinit();

	//static void update();

  private:

	friend struct Transaction;
	friend struct NonTransaction;
	friend class TransactionData;
	friend class Binder;
};

struct TransactionInvalid : public std::runtime_error
{
	TransactionInvalid( std::string m_sql_string ) :
	  std::runtime_error( fmt::format( "Transaction accessed while invalid: Last executed: {}", m_sql_string ) )
	{}
};

struct DbResults
{
	int rows_returned { 0 };
};

struct DbException : public std::runtime_error
{};

struct NoRows : public DbException
{};

#endif //ATLAS_DATABASE_HPP
