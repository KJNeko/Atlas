//
// Created by kj16609 on 3/10/23.
//

#ifndef ATLAS_SEARCH_HPP
#define ATLAS_SEARCH_HPP

#include <vector>

#include <QString>

#include "h95/database/Record.hpp"

class Search : public QObject
{
	Q_OBJECT

	std::string query {};

  public:

  signals:
	//! Emitted when a search is completed
	void searchCompleted( std::vector< Record > );

  public slots:
	//! Submits a text to get autocompleted.
	void searchTextChanged( QString text );
	void triggerEmptySearch();
};

#endif //ATLAS_SEARCH_HPP
