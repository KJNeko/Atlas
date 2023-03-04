//
// Created by kj16609 on 3/2/23.
//
#include "KeyReplacer.hpp"

#include <h95/logging.hpp>

QString KeyReplacer::value( const QString& key )
{
	const auto itter { key_list.find( key ) };
	if ( itter == key_list.end() )
		return {};
	else
		return itter->second;
}

void KeyReplacer::setKey( QString key, QString value )
{
	key_list.insert_or_assign( std::move( key ), std::move( value ) );
}

void KeyReplacer::replace( QString& str )
{
	for ( const auto& [key, text] : key_list ) { str.replace( key, text ); }
}

void KeyReplacer::extractKeys( std::filesystem::path text, std::filesystem::path pattern )
{
	const auto& str { text.string() };
	if ( str.ends_with( '/' ) ) text = str.substr( 0, str.size() - 1 );

	//Step counter to prevent infinite loops.
	uint16_t step_counter { 0 };
	while ( !pattern.empty() && step_counter < 512 )
	{
		const auto key { QString::fromStdString( pattern.filename().string() ) };
		pattern = pattern.parent_path();

		if ( pattern.empty() )
			key_list.insert_or_assign( key, QString::fromStdString( text.string() ) );
		else
			key_list.insert_or_assign( key, QString::fromStdString( text.filename().string() ) );

		text = text.parent_path();
		++step_counter;
	}
}
