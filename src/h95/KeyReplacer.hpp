//
// Created by kj16609 on 1/31/23.
//

#ifndef HYDRUS95_KEYREPLACER_HPP
#define HYDRUS95_KEYREPLACER_HPP

#include <unordered_map>
#include <QString>

class KeyReplacer
{
	std::unordered_map< QString, QString > key_map {};

	public:
	//! Registers a new key for the given value in the key_map
	void registerKey( const QString& key, QString value );

	//! Gathers keys from a string using a mask
	// Probably remake this later with some regex bullshittery.
	//void parseKeys(const QString& mask, const QString& str);

	//! Returns a value for the given key
	QString value( const QString& key ) const;

	/**
	 * @example {h95_games}/{title} -> /home/kj16609/h95/games/My Game Title
	 * @param str string to replace text within
	 * @return ref to str.
	 */
	//! Populates a string by replacing the keys given with the values
	QString& replaceKeys( QString& str ) const;

	void clear();
};

#endif	//HYDRUS95_KEYREPLACER_HPP
