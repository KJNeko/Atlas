//
// Created by kj16609 on 1/14/23.
//

#ifndef HYDRUS95_RECORDBASE_HPP
#define HYDRUS95_RECORDBASE_HPP

#include <QJsonObject>
#include <cstdint>

using RecordID = uint32_t;

//! Collects all key-values for the given record id
QJsonObject collectJson(const RecordID id);

class RecordBase
{
	RecordID m_id;

	QJsonObject record_json;

	public:
	RecordBase(const RecordID id);




};


#endif	//HYDRUS95_RECORDBASE_HPP
