//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_DATABASE_HPP
#define HYDRUS95_DATABASE_HPP

#include <filesystem>

#include <QObject>
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

#include <sqlite_modern_cpp.h>

#include <client/TracyLock.hpp>

#pragma GCC diagnostic pop

namespace internal
{
#ifdef TRACY_ENABLE
	using MtxType = tracy::Lockable<std::mutex>;
	using LockGuardType = std::lock_guard< tracy::Lockable<std::mutex> >;
#else
	using MtxType = std::mutex;
	using LockGuardType = std::lock_guard< std::mutex >;
#endif
}

class Database
{
	static sqlite::database& ref();

	static internal::MtxType& lock();

  public:

	static void initalize( const std::filesystem::path init_path );
	static void deinit();

	//static void update();

  private:

	friend struct Transaction;
	friend struct NonTransaction;
	friend class TransactionData;
};

struct TransactionInvalid : public std::runtime_error
{
	TransactionInvalid() : std::runtime_error( "Transaction accessed while invalid" ) {}
};

class TransactionData
{
	internal::LockGuardType guard;

	internal::LockGuardType getLock();

	//! True if operator<< has been called at least once
	bool ran_once { false };

	//! True if commit/abort has been called
	bool invalid { false };

  public:

	TransactionData();

	~TransactionData();

	friend struct Transaction;
	friend struct NonTransaction;
};

enum TransactionAutocommit
{
	Autocommit = true,
	NoAutocommit = false
};

//! Transaction unit to the database.
struct Transaction
{
	using enum TransactionAutocommit;

  private:

	Transaction* m_parent { nullptr };
	std::shared_ptr< TransactionData > data;
	bool m_autocommit { false };

	inline void releaseData()
	{
		data.reset();
		if ( m_parent != nullptr ) m_parent->releaseData();
	}

  public:

	//! @throws TransactionInvalid when trying to create a transaction without the database being initialized first
	Transaction() = delete;
	Transaction( const bool autocommit = false );
	Transaction( Transaction& other );
	Transaction( const Transaction& other ) = delete;
	Transaction( Transaction&& other ) = delete;
	Transaction& operator=( const Transaction& other ) = delete;

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
	std::unique_ptr<internal::LockGuardType> guard;

  public:

	NonTransaction();

	sqlite::database_binder operator<<( const std::string& sql );

	void commit();

	void abort();

	~NonTransaction();
};

#endif //HYDRUS95_DATABASE_HPP
