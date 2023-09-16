//
// Created by kj16609 on 9/4/23.
//

#ifndef ATLASGAMEMANAGER_DEV_HPP
#define ATLASGAMEMANAGER_DEV_HPP

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <tracy/Tracy.hpp>

//! Dev namespace is used for allowing data serialization in order to have the user be able to copy application state to the clipboard and send.
namespace atlas::logging::dev
{
	//To implement for each class
	template < typename T >
	[[nodiscard]] QJsonObject internalSerializer( const T& t );

	//! Injects information from T into the json
	template < typename T >
	inline void injectData( [[maybe_unused]] const T& t, [[maybe_unused]] QJsonObject& obj )
	{
		//const auto& typeinfo { typeid( T ) };
		//obj[ "type" ] = QString::fromUtf8( typeinfo.name() );
	}

	template < typename T >
	[[nodiscard]] inline QJsonObject serialize( const T& t )
	{
		QJsonObject obj { internalSerializer< T >( t ) };
		injectData< T >( t, obj );
		return obj;
	}

	template < typename T, typename... Ts >
	inline void serializeInto( QJsonArray& array, const T& t, const Ts&... left )
	{
		ZoneScoped;
		const auto data { serialize< T >( t ) };
		injectData( t, data ); // Insert things like type and pointer information into the object.

		array.push_back( std::move( data ) );

		if constexpr ( sizeof...( left ) > 1 )
			serializeInto( array, t, left... );
		else if ( sizeof...( left ) == 1 )
		{
			const auto data2 { serialize( left... ) };
			injectData( left..., data2 );
			array.push_back( std::move( data2 ) );
		}
	}

	template < typename... Ts >
		requires( sizeof...( Ts ) > 0 )
	inline QJsonDocument serializeObjects( const Ts&... items )
	{
		QJsonArray array;

		serializeInto( array, items... );

		QJsonDocument doc;
		doc.setArray( std::move( array ) );
		return doc;
	}

} // namespace atlas::logging::dev

#endif //ATLASGAMEMANAGER_DEV_HPP
