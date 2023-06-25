//
// Created by kj16609 on 5/24/23.
//

#ifndef ATLASGAMEMANAGER_COLUMN_HPP
#define ATLASGAMEMANAGER_COLUMN_HPP

#include <string>

#include "Transaction.hpp"

template < typename IdType, typename T >
struct DColumn
{
	const IdType m_id;
	std::optional< T > value;

  public:

	void set( T t )
	{
		ZoneScoped;
		RapidTransaction() << fmt::format( "UPDATE {} SET {} = ? WHERE {} = ?", table(), col(), keyCol() ) << m_id << t;
	}

	T get( bool use_cache = true )
	{
		ZoneScoped;
		if ( use_cache && value.has_value() )
			return value.value();
		else
		{
			T tmp_val;
			RapidTransaction() << fmt::format( "SELECT {} FROM {} WHERE {} = ?", col(), table(), keyCol() ) << m_id
				>> tmp_val;
			value = tmp_val;
			return std::move( tmp_val );
		}
	}

	void refresh() { value = get( false ); }

  private:

	std::optional< T > tryFetch()
	{
		try
		{
			return get( false );
		}
		catch ( ... )
		{
			return std::nullopt;
		}
	}

	virtual std::string_view table() = 0;
	virtual std::string_view col() = 0;
	virtual std::string_view keyCol() = 0;

  public:

	DColumn( const IdType id, const T val ) : m_id( id ), value( val ) {}

	DColumn( const IdType id, const bool immediate ) : m_id( id ), value( immediate ? tryFetch() : std::nullopt ) {}

	virtual ~DColumn() = default;
};

#define DEFINE_COL_STRUCT( type, col_name, our_type )                                                                  \
	struct our_type : public COL< type >                                                                               \
	{                                                                                                                  \
		std::string_view col() override                                                                                \
		{                                                                                                              \
			return col_name;                                                                                           \
		}                                                                                                              \
                                                                                                                       \
		our_type( const KEY_TYPE id, const bool immediate = false ) : COL< type >( id, immediate )                     \
		{}                                                                                                             \
		our_type( const KEY_TYPE id, type t ) : COL< type >( id, std::move( t ) )                                      \
		{}                                                                                                             \
	};

#define DEFINE_COL_BASE( table_name, key_name )                                                                        \
	template < typename T >                                                                                            \
	struct COL : public DColumn< KEY_TYPE, T >                                                                         \
	{                                                                                                                  \
		COL( const KEY_TYPE id, T t ) : DColumn< KEY_TYPE, T >( id, std::move( t ) )                                   \
		{}                                                                                                             \
                                                                                                                       \
		COL( const KEY_TYPE id, const bool immediate = false ) : DColumn< KEY_TYPE, T >( id, immediate )               \
		{}                                                                                                             \
                                                                                                                       \
	  private:                                                                                                         \
                                                                                                                       \
		std::string_view table() override                                                                              \
		{                                                                                                              \
			return table_name;                                                                                         \
		}                                                                                                              \
                                                                                                                       \
		std::string_view keyCol() override                                                                             \
		{                                                                                                              \
			return key_name;                                                                                           \
		}                                                                                                              \
	};

#endif //ATLASGAMEMANAGER_COLUMN_HPP
