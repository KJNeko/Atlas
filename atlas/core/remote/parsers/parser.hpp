//
// Created by kj16609 on 6/28/23.
//

#ifndef ATLASGAMEMANAGER_PARSER_HPP
#define ATLASGAMEMANAGER_PARSER_HPP

#include <QJsonObject>

namespace remote::parsers
{
	constexpr std::uint64_t MAX_REMOTE_VERSION { 1 };

	namespace v1
	{
		void processJson( const QJsonObject& json );
	}
} // namespace remote::parsers

#endif //ATLASGAMEMANAGER_PARSER_HPP
