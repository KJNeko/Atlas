//
// Created by kj16609 on 1/14/23.
//

#ifndef HYDRUS95_RECORDBASE_HPP
#define HYDRUS95_RECORDBASE_HPP

#include <QJsonObject>
#include <cstdint>

using RecordID = uint32_t;

/**
 * @page RecordJson Record Json
 *
 * Hydrus95 uses a key-value system in order to display relevent information to the user that can be modified or changed in how it is displayed. (Such as hiding or overiding information)
 *
 * The default information comes from the `User` parser. Which collects information from the user to display. Here are examples of the values the `User` parser can hold
 *
 * | key | type | default (Blank of optional) | description | Required |
 * |-----|------|---------|-------------|---------------------------|
 * | `name` | String | "New Record" | Name of the record | Yes |
 * | `version` | String |  | Version of the game/asset/item | Optional |
 * | `creator` | String |  | Creator of the game/asset/item | Optional |
 * | `rating` | float (0.0f-10.0f) | | Rating that the user can set | Optional |
 * | `installed` | bool | false | Indicates the user has the files associated with the record | Yes |
 *
 * For more information about what the pre-packaged parsers possess you can view the @ref ParserPage
 *
 */


class RecordBase
{
	RecordID m_id;

	QJsonObject record_json;
};


#endif	//HYDRUS95_RECORDBASE_HPP
