//
// Created by kj16609 on 3/10/23.
//

#ifndef HYDRUS95_SEARCH_HPP
#define HYDRUS95_SEARCH_HPP

#include <vector>

#include <QString>

#include "h95/database/Record.hpp"

//TODO: Fucking EVERYTHING

class Search : public QObject
{
	Q_OBJECT

	std::vector< QString > active_tags;

	public:
	signals:
	//! Emitted when a search is completed
	void searchCompleted( std::vector< Record > );
	//! Emitted when the autocomplete is finished
	void autoCompleteCompleted( std::vector< QString > );

	public slots:
	//! Submits a text to get autocompleted.
	void searchTextChanged( QString text );
	//! Adds a tag to the search. Does nothing if already present
	void addTag( QString );
	//! Removes a tag from the search. Does nothing if tag wasn't present
	void removeTag( QString );

	//! Returns a list of all the tags active in the search.
	const std::vector< QString >& getTagsActive() const;

	private:
	std::vector< Record > search();
};


#endif	//HYDRUS95_SEARCH_HPP
