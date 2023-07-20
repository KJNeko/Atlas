//
// Created by kj16609 on 7/20/23.
//

#include "core/database/record/Game.hpp"
#include "core/database/record/GameData.hpp"
#include "core/logging.hpp"

namespace atlas::logging::dev
{
	template <>
	QJsonObject internalSerializer< std::string_view >( const std::string_view& str )
	{
		QJsonObject obj;

		obj[ "string" ] = QString::fromUtf8( str.data(), static_cast< qsizetype >( str.size() ) );
		return obj;
	}

	template <>
	QJsonObject internalSerializer< std::filesystem::path >( const std::filesystem::path& path )
	{
		QJsonObject obj;

		obj[ "path" ] = QString::fromStdString( path.string() );
		obj[ "exists" ] = std::filesystem::exists( path );

		if ( std::filesystem::exists( path ) )
			obj[ "canonical" ] = QString::fromStdString( std::filesystem::canonical( path ).string() );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< std::vector< std::filesystem::path > >( const std::vector< std::filesystem::path >&
	                                                                            paths )
	{
		QJsonObject obj;

		for ( std::size_t i = 0; i < paths.size(); ++i ) obj[ QString::number( i ) ] = serialize( paths[ i ] );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< unsigned int >( const unsigned int& num )
	{
		QJsonObject obj;

		obj[ "value" ] = static_cast< qint64 >( num );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< unsigned long >( const unsigned long& num )
	{
		QJsonObject obj;

		obj[ "value" ] = static_cast< qint64 >( num );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< std::array< std::filesystem::path, BannerType::SENTINEL > >( const std::array<
																								 std::filesystem::path,
																								 BannerType::SENTINEL >&
	                                                                                                 array )
	{
		QJsonObject obj;

		for ( std::size_t i = 0; i < BannerType::SENTINEL; ++i ) obj[ QString::number( i ) ] = serialize( array[ i ] );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< std::vector< QString > >( const std::vector< QString >& paths )
	{
		QJsonObject obj;

		obj[ "size" ] = static_cast< qint64 >( paths.size() );
		for ( std::size_t i = 0; i < paths.size(); ++i ) obj[ QString::number( i ) ] = serialize( paths[ i ] );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< std::vector< Version > >( const std::vector< Version >& paths )
	{
		QJsonObject obj;

		obj[ "size" ] = static_cast< qint64 >( paths.size() );
		for ( std::size_t i = 0; i < paths.size(); ++i ) obj[ QString::number( i ) ] = serialize( paths[ i ] );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< QString >( const QString& str )
	{
		QJsonObject obj;

		obj[ "value" ] = str;

		return obj;
	}

	template <>
	QJsonObject internalSerializer< std::shared_ptr< VersionData > >( const std::shared_ptr< VersionData >& shared_ptr )
	{
		QJsonObject obj;

		obj[ "internal_ptr" ] = reinterpret_cast< qint64 >( shared_ptr.get() );
		obj[ "ref count" ] = static_cast< qint64 >( shared_ptr.use_count() );
		obj[ "data" ] = serialize< VersionData >( *shared_ptr.get() );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< Version >( const Version& version )
	{
		QJsonObject obj;

		obj[ "m_parent_id" ] = static_cast< qint64 >( version.m_parent_id );
		obj[ "m_version" ] = version.m_version;
		obj[ "data_ptr" ] = serialize( version.data_ptr );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< VersionData >( const VersionData& data )
	{
		QJsonObject obj;

		obj[ "m_parent_id" ] = static_cast< qint64 >( data.m_parent_id );
		obj[ "m_version" ] = data.m_version;
		obj[ "m_game_path" ] = serialize( data.m_game_path );
		obj[ "m_exec_path" ] = serialize( data.m_exec_path );
		obj[ "m_last_played" ] = static_cast< qint64 >( data.m_last_played );
		obj[ "m_total_playtime" ] = static_cast< qint64 >( data.m_total_playtime );
		obj[ "m_folder_size" ] = static_cast< qint64 >( data.m_folder_size );
		obj[ "m_date_added" ] = static_cast< qint64 >( data.m_date_added );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< GameData >( const GameData& data )
	{
		QJsonObject obj;

		obj[ "m_game_id" ] = static_cast< qint64 >( data.m_game_id );
		obj[ "m_title" ] = data.m_title;
		obj[ "m_creator" ] = data.m_creator;
		obj[ "m_engine" ] = data.m_engine;
		obj[ "m_last_played" ] = static_cast< qint64 >( data.m_last_played );
		obj[ "m_total_playtime" ] = static_cast< qint64 >( data.m_total_playtime );
		obj[ "m_preview_count" ] = static_cast< qint64 >( data.m_preview_count );
		obj[ "m_tags" ] = serialize< decltype( data.m_tags ) >( data.m_tags );
		obj[ "m_description" ] = data.m_description;
		obj[ "m_versions" ] = serialize< decltype( data.m_versions ) >( data.m_versions );
		obj[ "m_banner_paths" ] = serialize< decltype( data.m_banner_paths ) >( data.m_banner_paths );
		obj[ "m_preview_paths" ] = serialize< decltype( data.m_preview_paths ) >( data.m_preview_paths );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< std::shared_ptr< GameData > >( const std::shared_ptr< GameData >& shared_ptr )
	{
		QJsonObject obj;

		obj[ "internal_ptr" ] = reinterpret_cast< qint64 >( shared_ptr.get() );
		obj[ "ref count" ] = static_cast< qint64 >( shared_ptr.use_count() );
		obj[ "data" ] = serialize< GameData >( *shared_ptr.get() );

		return obj;
	}

	template <>
	QJsonObject internalSerializer< Game >( const Game& game )
	{
		QJsonObject obj;

		obj[ "m_ptr" ] = serialize( game.ptr );
		obj[ "m_id" ] = static_cast< qint64 >( game.m_id );

		return obj;
	}

} // namespace atlas::logging::dev
