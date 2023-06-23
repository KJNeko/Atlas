//
// Created by kj16609 on 6/23/23.
//

#ifndef ATLASGAMEMANAGER_TRANSACTION_HPP
#define ATLASGAMEMANAGER_TRANSACTION_HPP

#include <tracy/Tracy.hpp>

#include "Binder.hpp"

//! Internal class used for Transaction and NonTransaction's shared data
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

enum TransactionType
{
	Autocommit = 0,
	NoAutocommit,
	ReadOnly,
	NonTransaction
};

//! Transaction unit to the database.
struct Transaction
{
	using enum TransactionType;

  private:

	Transaction* m_parent { nullptr };
	std::shared_ptr< TransactionData > data;
	TransactionType m_type;
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

	/**
	 * @param autocommit if commit() should be called on dtor, Otherwise abort() is called if not called previously
	 */
	explicit Transaction( const TransactionType type );
	Transaction( Transaction& other );
	Transaction( const Transaction& other ) = delete;
	Transaction( Transaction&& other ) = delete;
	Transaction& operator=( const Transaction& other ) = delete;

	//! @throws TransactionInvalid
	Binder operator<<( std::string sql );

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
#endif //ATLASGAMEMANAGER_TRANSACTION_HPP
