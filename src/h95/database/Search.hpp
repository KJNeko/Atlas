//
// Created by kj16609 on 3/10/23.
//

#ifndef HYDRUS95_SEARCH_HPP
#define HYDRUS95_SEARCH_HPP

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
};

#endif //HYDRUS95_SEARCH_HPP
