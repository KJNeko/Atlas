//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_GAMERECORD_HPP
#define HYDRUS95_GAMERECORD_HPP

#include <QString>

using RecordID = uint32_t;

struct GameRecord
{
	RecordID m_id;

	GameRecord(RecordID id) : m_id(id) {}

	//! Execute the game
	void execute_game() const { return; }

	//! String must contain `%command%` or be blank
	void setExecuteString( const QString );

	bool setPlayStatus( bool played );
	bool setInstalledStatus( bool installed );

	void setVersion( QString ver_str );
	QString getVersion() const;
};


#endif	//HYDRUS95_GAMERECORD_HPP
