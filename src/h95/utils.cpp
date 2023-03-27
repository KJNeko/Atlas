//
// Created by kj16609 on 3/27/23.
//

#include "utils.hpp"

#include <QDirIterator>

std::size_t folderSize( const std::filesystem::path& folder )
{
	QDirIterator iter { folder, QDirIterator::Subdirectories };
	uint64_t counter { 0 };
	while ( iter.hasNext() ) counter += static_cast< std::size_t >( iter.nextFileInfo().size() );
	return counter;
}
