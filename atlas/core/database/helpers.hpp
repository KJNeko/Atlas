//
// Created by kj16609 on 5/24/23.
//

#ifndef ATLASGAMEMANAGER_HELPERS_HPP
#define ATLASGAMEMANAGER_HELPERS_HPP

#include <string>

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

template < typename CType, typename IDType, const bool cached = false >
class Column
{
	std::enable_if< cached, std::optional< CType > > m_cached_value {};

	const RecordID m_record_id;
	const std::string_view col_name {};

	const std::string fetch_sql;
	const std::string update_sql;

  public:

	Column(
		const std::string_view name, const std::string_view table, const std::string_view id_col, const IDType id ) :
	  m_record_id( id ),
	  col_name( name ),
	  fetch_sql( fmt::format( "SELECT {} FROM {} WHERE {} = {}", col_name, table, id_col, id ) ),
	  update_sql( fmt::format( "UPDATE {} SET {} = ? WHERE {} = {}", table, col_name, id_col, id ) )
	{}

	//! Gets the value (from cache if possible)
	CType get( Transaction transaction = Transaction( Autocommit ) )
	{
		if constexpr ( cached )
		{
			if ( m_cached_value.has_value() ) return m_cached_value.value();
		}

		typename ColIntermediate< CType >::Type tmp;

		transaction << fetch_sql >> tmp;

		if constexpr ( cached ) m_cached_value = tmp;

		return ColIntermediate< CType >::from( tmp );
	}

	//! Sets the value
	template < typename In >
	void set( const In& type, Transaction transaction = Transaction( Autocommit ) )
	{
		if constexpr ( cached )
		{
			m_cached_value = type;
		}

		transaction << update_sql << ColIntermediate< CType >::to( type );
	}

	//! Wipes the cache
	void wipe()
	{
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

/*
struct Example
{
	RecordID m_id;

	Column< std::string, RecordID > creator { "creator", "records", "record_id", m_id };
	Column< std::uint64_t, RecordID > last_playted { "last_played", "records", "record_id", m_id };
};
*/

#endif //ATLASGAMEMANAGER_HELPERS_HPP
