//
// Created by kj16609 on 6/28/23.
//

#ifndef ATLASGAMEMANAGER_PARSER_HPP
#define ATLASGAMEMANAGER_PARSER_HPP

#include <QJsonObject>

namespace remote::parsers
{

	//This is clearly used. Yet gcc thinks it is not
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-const-variable"
	constexpr std::uint64_t MAX_REMOTE_VERSION { 0 };
#pragma GCC diagnostic pop
#else
	constexpr std::uint64_t MAX_REMOTE_VERSION { 0 };
#endif

	namespace v0
	{
		void processJson( const QJsonObject& json );
	}
} // namespace remote::parsers

#endif //ATLASGAMEMANAGER_PARSER_HPP
