//
// Created by kj16609 on 1/27/23.
//

#ifndef ATLAS_LOGGING_HPP
#define ATLAS_LOGGING_HPP

#include <filesystem>

#include <QString>

#ifdef __GNUC__
#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#ifdef __linux__
#pragma GCC diagnostic ignored "-Wdangling-reference"
#endif
#endif

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include "Types.hpp"

void initLogging();

//! Formatter for QString in fmt::format
template <>
struct fmt::formatter< QString >
{
	constexpr auto parse( fmt::format_parse_context& ctx ) -> decltype( ctx.begin() ) { return ctx.begin(); }

	template < typename FormatContext >
	auto format( const QString& my, FormatContext& ctx ) const -> decltype( ctx.out() )
	{
		return fmt::format_to( ctx.out(), "{}", my.toStdString() );
	}
};

template <>
struct fmt::formatter< std::filesystem::path >
{
	bool print_canonical { false };
	bool print_exists { false };

	constexpr format_parse_context::iterator parse( format_parse_context& ctx )
	{
		//Check if ctx has 'c' 'ce' or 'e' and return the itterator after it
		auto idx { ctx.begin() };
		const auto end { ctx.end() };

		if ( idx != end && *idx == 'c' )
		{
			print_canonical = true;
			++idx;
		}

		if ( idx != end && *idx == 'e' )
		{
			print_exists = true;
			++idx;
		}

		return idx;
	}

	format_context::iterator format( const std::filesystem::path& path, format_context& ctx ) const;
};

namespace atlas::logging
{
	//! Loads the spdlog interfaces for file rotations and logging without a GUI
	void init();

	//! Loads the GUI hooks required for some warnings to display to the user
	void initGUIHooks();

	//! Notifies the user of a warning.
	inline void userwarn( [[maybe_unused]] std::string message )
	{
		spdlog::warn( "userwarn: {}", message );
		qDebug() << "userwarn: STUB";
	}

	//! Dev namespace is used for allowing data serialization in order to have the user be able to copy application state to the clipboard and send.
	namespace dev
	{
		//To implement for each class
		template < typename T >
		QJsonObject internalSerializer( const T& t );

		//! Injects information from T into the json
		template < typename T >
		inline void injectData( [[maybe_unused]] const T& t, [[maybe_unused]] QJsonObject& obj )
		{
			//const auto& typeinfo { typeid( T ) };
			//obj[ "type" ] = QString::fromUtf8( typeinfo.name() );
		}

		template < typename T >
		inline QJsonObject serialize( const T& t )
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
		inline QJsonDocument serializeObjects( const Ts&... items )
		{
			QJsonArray array;

			serializeInto( array, items... );

			QJsonDocument doc;
			doc.setArray( std::move( array ) );
			return doc;
		}

		template < typename... Ts >
		inline void error( std::string msg, const Ts&... ts )
		{
			spdlog::error( "{}: {}", msg, serializeObjects( ts... ).toJson().toStdString() );
		}

		template <>
		inline void error<>( std::string msg )
		{
			spdlog::error( "{}", msg );
		}

	} // namespace dev

} // namespace atlas::logging

#endif //ATLAS_LOGGING_HPP
