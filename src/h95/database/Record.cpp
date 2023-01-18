//
// Created by kj16609 on 1/15/23.
//

#include "Record.hpp"
#include <QSqlQuery>
#include <QJsonArray>
#include <iostream>
#include <QSqlError>
#include <QJsonObject>
#include <QJsonDocument>


QString Record::tableQuery()
{
	return "CREATE TABLE IF NOT EXISTS records (id INTEGER UNIQUE PRIMARY KEY, type, banner, previews, unique(id, type))";
}

void Record::bindTo( QSqlQuery& query ) const
{
	query.bindValue( ":id", m_id );
	query.bindValue( ":m_type", static_cast< int >( m_type ) );
	query.bindValue( ":banner", m_banner.c_str() );

	QJsonArray json;
	for ( const auto& preview : m_previews ) json.push_back( preview.c_str() );
	query.bindValue( ":previews", json );
}

bool Record::operator==( const Record& other ) const
{
#ifndef NDEBUG
	const bool value { other.m_id == this->m_id };

	if ( other.m_type == this->m_type
		 && ( ( std::holds_alternative< EmptyMetadata >( other.m_metadata )
				&& std::holds_alternative< EmptyMetadata >( this->m_metadata )
				&& std::get< EmptyMetadata >( other.m_metadata ) == std::get< EmptyMetadata >( this->m_metadata ) )
			  || ( std::holds_alternative< GameMetadata >( other.m_metadata )
				   && std::holds_alternative< GameMetadata >( this->m_metadata )
				   && std::get< GameMetadata >( other.m_metadata ) == std::get< GameMetadata >( this->m_metadata ) ) ) )
	{
		std::cout << "WARN! Values of metadata with same ID are not equal!!!" << std::endl;
	}

	return value;
#else
	return other.m_id == this->m_id;
#endif
}

Record Record::create(
	const RecordType type,
	const Metadata& metadata,
	const std::filesystem::path& banner,
	const std::vector< std::filesystem::path >& previews )
{
	QSqlQuery query;
	query.prepare( "INSERT INTO records (type, banner, previews) VALUES (:type, :banner, :previews) RETURNING id" );
	query.bindValue( ":type", static_cast< int >( type ) );
	query.bindValue( ":banner", banner.c_str() );

	QJsonArray json_array;
	for ( const auto& preview : previews ) json_array.push_back( preview.c_str() );

	QJsonDocument doc { json_array };

	query.bindValue( ":previews", doc.toJson( QJsonDocument::Compact ) );
	query.exec();
	query.first();

	std::cout << "executed: " << query.executedQuery().toStdString() << std::endl;

	const auto id { query.value( 0 ).value< RecordID >() };
	return { id, type, Metadata::insert( id, metadata, type ), banner, previews };
}

Record Record::select( const RecordID id )
{
	QSqlQuery query;
	query.prepare( "SELECT :type, :banner, :previews FROM records WHERE id = :id" );

	RecordType type {INVALID_RECORD};
	QString banner {};
	QJsonArray json {};

	query.bindValue( ":type", static_cast<unsigned int>(type), QSql::Out );
	query.bindValue( ":banner", banner, QSql::Out );
	query.bindValue( ":previews", json, QSql::Out );
	query.bindValue( ":id", id );
	query.exec();
	query.first();

	std::vector< std::filesystem::path > paths;
	for ( const auto& j : json ) paths.emplace_back( j.toString().toStdString() );

	return { id, type, Metadata::select( id, type ), banner.toStdString(), paths };
}
