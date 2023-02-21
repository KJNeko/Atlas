//
// Created by kj16609 on 1/31/23.
//

#include <QHash>

#include <h95/logging.hpp>

#include <h95/KeyReplacer.hpp>

void KeyReplacer::clear()
{
	key_map.clear();
}


QString KeyReplacer::value( const QString& key ) const
{
	spdlog::debug( "Getting value for key {}", key.toStdString() );
	if ( const auto& key_value = key_map.find( key ); key_value != key_map.end() )
		return key_value->second;
	else
		return {};
}


void KeyReplacer::registerKey( const QString& key, QString value )
{
	spdlog::debug( "Registering key {} to value {}", key.toStdString(), value.toStdString() );

	if ( const auto key_value = key_map.find( key ); key_value != key_map.end() )
		key_map.at( key ) = std::move( value );
	else
		key_map.emplace( key, std::move( value ) );
}

QString& KeyReplacer::replaceKeys( QString& str ) const
{
	for ( const auto& [key, value] : key_map ) str.replace( key, value );

	return str;
}
