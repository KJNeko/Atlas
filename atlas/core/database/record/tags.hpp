//
// Created by kj16609 on 7/29/23.
//

#ifndef ATLASGAMEMANAGER_TAGS_HPP
#define ATLASGAMEMANAGER_TAGS_HPP

#include <cstdint>

class QString;

using TagID = std::uint64_t;
[[maybe_unused]] constexpr TagID INVALID_TAG_ID { 0 };

namespace atlas::tags
{
	//! Creates a new tag (or returns an existing ID)
	TagID createTag( const QString& str );

} // namespace atlas::tags

#endif //ATLASGAMEMANAGER_TAGS_HPP
