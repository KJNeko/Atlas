//
// Created by kj16609 on 3/10/23.
//

#ifndef ATLAS_SEARCH_HPP
#define ATLAS_SEARCH_HPP

#include <QString>

#include <vector>

#include "core/database/record/Game.hpp"
#include "core/search/QueryBuilder.hpp"

class Search final : public QObject
{
	Q_OBJECT

	std::string query {};

  public:

  signals:
	//! Emitted when a search is completed
	void searchCompleted( std::vector< atlas::records::Game > );

  public slots:
	//! Submits a text to get autocompleted.
	void searchTextChanged( QString text ); //, const SortOrder order, const bool asc );
};

#endif //ATLAS_SEARCH_HPP
