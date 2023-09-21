//
// Created by kj16609 on 9/21/23.
//

#include <QString>

#include "Game.hpp"
#include "core/database/RapidTransaction.hpp"
#include "core/database/record/GameData.hpp"

namespace atlas::records
{
	void Game::addUserTag( QString str )
	{
		auto& tags { ptr->m_tags };
		if ( const auto itter = std::find( tags.begin(), tags.end(), str ); itter == tags.end() )
		{
			std::uint64_t tag_id { 0 };
			RapidTransaction() << "SELECT tag_id FROM tags WHERE tag = ?" << str >> tag_id;

			if ( tag_id == 0 )
			{
				RapidTransaction() << "INSERT INTO tags (tag) VALUES (?) RETURNING tag_id" << str >> tag_id;
			}

			RapidTransaction() << "INSERT INTO tag_mappings (tag_id, record_id) VALUES (?,?)" << tag_id << m_id;
			tags.emplace_back( str );
		}
	}

	void Game::removeUserTag( QString str )
	{
		auto& tags { ptr->m_tags };
		if ( const auto itter = std::find( tags.begin(), tags.end(), str ); itter != tags.end() )
		{
			RapidTransaction() << "DELETE FROM tag_mappings NATURAL JOIN WHERE tag = ? AND record_id = ?" << str
							   << m_id;
			tags.erase( itter );
		}
	}

} // namespace atlas::records
