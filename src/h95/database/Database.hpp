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
	static void initalize( const std::filesystem::path init_path );
	static void deinit();

	//static void update();

	private:
	friend struct Transaction;
	friend struct NonTransaction;
	friend struct TransactionData;
};

struct TransactionInvalid : public std::runtime_error
{
	TransactionInvalid() : std::runtime_error( "Transaction accessed while invalid" ) {}
};

struct TransactionData
{
	bool finished { false };
	std::lock_guard< std::mutex > guard;

	private:

	std::lock_guard<std::mutex> getLock();

	public:

	TransactionData();

	~TransactionData();
};

//! Transaction unit to the database.
struct Transaction
{
	private:
	std::shared_ptr< TransactionData > data;
	bool m_autocommit { false };

	public:
	//! @throws TransactionInvalid when trying to create a transaction without the database being initialized first
	Transaction( const bool autocommit = false );
	Transaction( const Transaction& other ) = default;
	Transaction( Transaction&& other ) = default;

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

struct NonTransaction
{
	Q_DISABLE_COPY_MOVE( NonTransaction )

	private:
	bool finished { false };
	std::lock_guard< std::mutex >* guard { nullptr };

	public:
	NonTransaction();

	sqlite::database_binder operator<<( const std::string& sql );

	void commit();

	void abort();

	~NonTransaction();
};

inline sqlite::database_binder& operator<<( sqlite::database_binder&& db, QString& str )
{
	return db << ( str.toStdString() );
}

inline sqlite::database_binder& operator<<( sqlite::database_binder& db, QString& str )
{
	return db << ( str.toStdString() );
}

#endif	//HYDRUS95_DATABASE_HPP
