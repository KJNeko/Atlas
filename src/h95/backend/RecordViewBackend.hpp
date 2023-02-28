//
// Created by kj16609 on 1/18/23.
//

#ifndef HYDRUS95_RECORDVIEWBACKEND_HPP
#define HYDRUS95_RECORDVIEWBACKEND_HPP

#include <QObject>

#include <h95/database/Record.hpp>
#include <h95/database/Tag.hpp>

class RecordViewBackend final : public QObject
{
	Q_OBJECT

	std::vector< Tag > active_tags {};

	public:
	public slots:
	//! Adds a tag to the taglist
	void addTag( const Tag& tag );
	//! Removes a tag from the taglist
	void removeTag( const Tag& tag );

	void refresh();

	std::vector< Tag > getTags() const;

	signals:
	//! Emitted when a search for similar tags is done
	void similarTagFinished( const std::vector< Tag >& tags );

	//! Emitted when the tag list has changed
	void tagsChanged( const std::vector< Tag >& tags );

	//! Emitted when a search is completed for tags or when a new record is added
	void recordsUpdated( const std::vector< Record >& records );
};


#endif	//HYDRUS95_RECORDVIEWBACKEND_HPP
