//
// Created by kj16609 on 3/2/23.
//

#ifndef HYDRUS95_KEYREPLACER_HPP
#define HYDRUS95_KEYREPLACER_HPP

#include <QString>
#include <QObject>

#include <unordered_map>
#include <filesystem>

/*
std::unordered_map< QString, QString > extractKeys( std::filesystem::path key_set, std::filesystem::path path )
{

}*/

class KeyReplacer
{
	std::unordered_map< QString, QString > key_list {};

	public:
	QString value( const QString& key );

	void setKey( QString key, QString value );

	//! Replaces all keys with the value found in key_list
	void replace( QString& str );

	friend class TestKeyReplacer;
	void extractKeys( std::filesystem::path text, std::filesystem::path pattern );
};

#endif	//HYDRUS95_KEYREPLACER_HPP
