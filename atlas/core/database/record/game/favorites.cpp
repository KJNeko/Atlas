//
// Created by kj16609 on 12/28/23.
//

#include "Game.hpp"
#include "core/database/RapidTransaction.hpp"
#include "core/database/record/GameData.hpp"

namespace atlas::records
{

	void Game::setFavorite( const bool state )
	{
		if ( state )
			atlas::database::RapidTransaction() << "INSERT INTO favorites (record_id) VALUES (?);" << m_id;
		else
			atlas::database::RapidTransaction() << "DELETE FROM favorites WHERE record_id = ?;" << m_id;

		ptr->m_favorite = state;
	}

} // namespace atlas::records
