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

struct TransactionBase;

class Database
{
	static sqlite::database& ref();
	static std::mutex& lock();

	public:
	static void initalize( const std::filesystem::path init_path = "./data/hydrus95.db" );
	static void deinit();

	//static void update();

	private:
	friend class TestDatabase;
	friend struct TransactionBase;
	friend class TestRecord;
};

struct TransactionInvalid : public std::runtime_error
{
	TransactionInvalid() : std::runtime_error( "Transaction accessed while invalid" ) {}
};

struct TransactionBase
{
	Q_DISABLE_COPY_MOVE( TransactionBase )

	private:
	bool finished { false };
	std::lock_guard< std::mutex >* guard { nullptr };

	public:
	TransactionBase();

	sqlite::database& ref();

	void commit();
	void abort();

	~TransactionBase();
};

using Transaction = TransactionBase;

#endif	//HYDRUS95_DATABASE_HPP
