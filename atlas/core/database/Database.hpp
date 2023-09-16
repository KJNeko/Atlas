//
// Created by kj16609 on 1/12/23.
//

#pragma once
#ifndef ATLAS_DATABASE_HPP
#define ATLAS_DATABASE_HPP

#include <filesystem>
#include <mutex>
#include <sqlite3.h>

namespace internal
{
	using MtxType = std::mutex;
	using LockGuardType = std::lock_guard< std::mutex >;
} // namespace internal

class Database
{
	//! Returns a ref to the global DB lock
	[[nodiscard]] static internal::MtxType& lock();

  public:

	//! Initalizes the database with init_path. Does not have to be caonical
	static void initalize( const std::filesystem::path init_path );
	//! Deinitalizes the DB.
	static void deinit();

	//static void update();

	//! Returns a ref to the sqlite DB
	static sqlite3& ref();

  private:

	friend class Binder;
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
