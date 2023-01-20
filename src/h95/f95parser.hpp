//
// Created by kj16609 on 1/16/23.
//

#ifndef HYDRUS95_F95PARSER_HPP
#define HYDRUS95_F95PARSER_HPP

#include <cstdint>
#include <vector>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>

using F95ThreadID = uint32_t;
using F95PrefixID = uint16_t;
using F95TagID = uint32_t;

struct F95Parser
{
	F95ThreadID thread_id;
	QString title;
	QString creator;
	QString version;
	std::vector< F95PrefixID > prefixes;
	std::vector< F95TagID > tags;
	std::filesystem::path cover;
	std::vector< std::filesystem::path > previews;
	bool new_entry;

	//! See @refitem F95Json
	F95Parser( const QJsonObject& json )
	{
		thread_id = json["thread_id"].toInt();
		title = json["title"].toString();
		creator = json["creator"].toString();
		version = json["version"].toString();

		for ( const auto& prefix : json["prefixes"].toArray() ) prefixes.emplace_back( prefix.toInt() );

		for ( const auto& tag : json["tags"].toArray() ) tags.emplace_back( tag.toInt() );

		cover = json["cover"].toString().toStdString();

		const QJsonArray screen_array = json["screens"].toArray();

		for ( const auto& screen : screen_array ) previews.emplace_back( screen.toString().toStdString() );

		new_entry = json["new"].toBool();
	}
};

#endif	//HYDRUS95_F95PARSER_HPP
