//
// Created by kj16609 on 1/13/23.
//

#ifndef HYDRUS95_RECORDFLYWEIGHT_HPP
#define HYDRUS95_RECORDFLYWEIGHT_HPP

#include "RecordBase.hpp"
#include <memory>

class RecordFlyweight : public std::shared_ptr<RecordBase>
{
	RecordID m_id;

	public:
	RecordFlyweight() = delete;
	RecordFlyweight(RecordID id);
	RecordFlyweight(const RecordFlyweight& other) = default;
	RecordFlyweight(RecordFlyweight&& other) = default;
	RecordFlyweight& operator=(const RecordFlyweight&) = default;

	~RecordFlyweight() noexcept;
};

using Record = RecordFlyweight;



#endif	//HYDRUS95_RECORDFLYWEIGHT_HPP
