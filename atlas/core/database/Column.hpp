//
// Created by kj16609 on 5/24/23.
//

#ifndef ATLASGAMEMANAGER_COLUMN_HPP
#define ATLASGAMEMANAGER_COLUMN_HPP

#include <string>

#include "Transaction.hpp"

template < typename T >
struct ColIntermediate
{
	using Type = T;

	static Type to( const T& t ) { return t; }

	static T from( const Type& t ) { return t; }
};

template <>
struct ColIntermediate< QString >
{
	using Type = std::string;

	static std::string to( const QString& str ) { return str.toStdString(); }

	static QString from( const std::string& str ) { return QString::fromStdString( str ); }
};

template < typename T >
concept has_size_func = requires( T t ) {
	{
		t.size()
	} -> std::convertible_to< std::uint64_t >;
};

template < typename T >
concept has_at_assign = requires( T t ) {
	{
		t.at( 0 ) = {}
	};
};

template < typename T >
concept has_emplace = requires( T t ) {
	{
		t.emplace_back( {} )
	};
};

template < typename CType, typename IDType, const bool cached = false >
class Column
{
	std::enable_if< cached, std::optional< CType > > m_cached_value {};
	std::enable_if< cached, std::chrono::time_point< std::chrono::steady_clock > > m_last_accessed {};

	const std::string_view col_name {};

	const std::string fetch_sql {};
	const std::string update_sql {};

  public:

	Column(
		const std::string_view name, const std::string_view table, const std::string_view id_col, const IDType id ) :
	  col_name( name ),
	  fetch_sql( fmt::format( "SELECT {} FROM {} WHERE {} = {}", col_name, table, id_col, id ) ),
	  update_sql( fmt::format( "UPDATE {} SET {} = ? WHERE {} = {}", table, col_name, id_col, id ) )
	{}

	//! Gets the value (from cache if possible)
	template < typename T = CType >
	T get( Transaction transaction = Transaction( Autocommit ) )
	{
		ZoneScoped;
		if constexpr ( cached )
		{
			m_last_accessed = std::chrono::steady_clock::now();
			if ( m_cached_value.has_value() ) return m_cached_value.value();
		}

		typename ColIntermediate< T >::Type tmp;

		transaction << fetch_sql >> tmp;

		if constexpr ( cached ) m_cached_value = tmp;

		return ColIntermediate< T >::from( tmp );
	}

	//! Sets the value
	void set( const CType& type, Transaction transaction = Transaction( Autocommit ) )
	{
		ZoneScoped;
		if constexpr ( cached )
		{
			m_cached_value = type;
		}

		transaction << update_sql << ColIntermediate< CType >::to( type );
	}

	//! Wipes the cache
	void wipe()
	{
		ZoneScoped;
		if constexpr ( cached )
		{
			//Can wipe
			m_cached_value = std::nullopt;
			return;
		}
		else
		{
			return;
		}
	}
};

template < typename CType, typename IDType, const bool cached >
class Column< std::vector< CType >, IDType, cached >
{
	std::enable_if< cached, std::vector< CType > > m_cached_value {};
	std::enable_if< cached, std::chrono::time_point< std::chrono::steady_clock > > m_last_accessed;

	const std::string_view col_name {};

	const std::string fetch_sql {};
	const std::string insert_sql {};
	const std::string delete_sql {};

  public:

	Column(
		const std::string_view name, const std::string_view table, const std::string_view id_col, const IDType id ) :
	  col_name( name ),
	  fetch_sql( fmt::format( "SELECT {} FROM {} WHERE {} = {}", col_name, table, id_col, id ) ),
	  insert_sql( fmt::format( "INSERT INTO {} ({}, {}) VALUES ({}, {})", table, id_col, col_name, id, col_name ) ),
	  delete_sql( fmt::format( "DELETE FROM {} WHERE {} = {}", table, id_col, id ) )
	{}

	//! Gets the value (from cache if possible)
	template < typename T = CType >
	std::vector< T > get( Transaction transaction = Transaction( Autocommit ) )
	{
		ZoneScoped;
		if constexpr ( cached )
		{
			m_last_accessed = std::chrono::steady_clock::now();
			if ( m_cached_value.has_value() ) return m_cached_value.value();
		}

		using Intermediate = ColIntermediate< CType >::Type;

		std::vector< T > tmp;

		transaction << fetch_sql >> [ &tmp ]( const Intermediate val )
		{ tmp.emplace_back( ColIntermediate< CType >::from( val ) ); };

		if constexpr ( cached ) m_cached_value = tmp;

		return tmp;
	}

	//! Sets the value
	void add( const CType& type, Transaction transaction = Transaction( Autocommit ) )
	{
		ZoneScoped;
		if constexpr ( cached )
		{
			//Check if it exists already
			if ( std::find( m_cached_value.begin(), m_cached_value.end(), type ) != m_cached_value.end() )
				return;
			else
				m_cached_value.emplace_back( type );
		}
		else
		{
			const auto tmp { get( transaction ) };
			if ( std::find( tmp.begin(), tmp.end(), type ) != tmp.end() ) return;
		}

		transaction << insert_sql << ColIntermediate< CType >::to( type );
	}

	void remove( const CType& type, Transaction transaction = Transaction( Autocommit ) )
	{
		ZoneScoped;

		if constexpr ( cached )
		{
			const auto idx { std::find( m_cached_value.begin(), m_cached_value.end(), type ) };
			if ( idx == m_cached_value.end() ) return;
			m_cached_value.erase( idx );
		}

		transaction << delete_sql << ColIntermediate< CType >::to( type );
	}

	//! Wipes the cache
	void wipe()
	{
		ZoneScoped;
		if constexpr ( cached )
		{
			//Can wipe
			m_cached_value = std::nullopt;
			return;
		}
		else
		{
			return;
		}
	}
};

template < typename CType, typename IDType >
using CachedColumn = Column< CType, IDType, true >;

template < typename CType, typename IDType, const bool is_cached = false >
using ListColumn = Column< std::vector< CType >, IDType, is_cached >;

template < typename CType, typename IDType >
using CachedListColumn = ListColumn< std::string, RecordID, false >;

/*
struct Example
{
	RecordID m_id;

	Column< std::string, RecordID > creator { "creator", "records", "record_id", m_id };
	Column< std::uint64_t, RecordID > last_playted { "last_played", "records", "record_id", m_id };
};
*/

#endif //ATLASGAMEMANAGER_COLUMN_HPP
