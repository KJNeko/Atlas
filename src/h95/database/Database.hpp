//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_DATABASE_HPP
#define HYDRUS95_DATABASE_HPP

#include <filesystem>

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wconversion"

#include <sqlite_modern_cpp.h>

#pragma GCC diagnostic pop

#include <QObject>

namespace sqlite
{
	class database;
}

class Database
{
	static sqlite::database& ref();
	static std::mutex& lock();

	public:
	static void initalize( const std::filesystem::path init_path = "./data/hydrus95.db" );
	static void deinit();

	//static void update();

	private:
	friend struct Transaction;
};

struct TransactionInvalid : public std::runtime_error
{
	TransactionInvalid() : std::runtime_error( "Transaction accessed while invalid" ) {}
};

//! Transaction unit to the database.
struct Transaction
{
	Q_DISABLE_COPY_MOVE( Transaction )

	private:
	bool finished { false };
	std::lock_guard< std::mutex >* guard { nullptr };

	public:
	//! @throws TransactionInvalid when trying to create a transaction without the database being initalized first
	Transaction();

	//! @throws TransactionInvalid
	sqlite::database_binder operator<<( const std::string& sql );

	/**
	 * @brief Commits the transaction. Committing all changes made by this transaction
	 * @throws TransactionInvalid when attempting to call commit() after an abort() or commit() twice
	 */
	void commit();

	/**
	 * @brief Aborts the transaction. Reverting all changes made by this transaction
	 * @throws TransactionInvalid when attempting to call abort() after a commit() or abort() twice
	 */
	void abort();

	~Transaction();
};

#endif	//HYDRUS95_DATABASE_HPP
