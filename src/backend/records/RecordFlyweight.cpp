//
// Created by kj16609 on 1/13/23.
//

#include "RecordFlyweight.hpp"

namespace internal
{
	inline static std::unordered_map<RecordID, std::shared_ptr<RecordBase>> map;
	inline static std::mutex map_mtx;

	std::shared_ptr<RecordBase> getSharedPtr(const RecordID id)
	{
		std::lock_guard guard(map_mtx);
		const auto itter {map.find(id)};
		if(itter != map.end())
			return itter->second;
		else
		{
			std::shared_ptr<RecordBase> ptr {std::make_shared<RecordBase>(id)};
			map.emplace(id, ptr);
			return ptr;
		}
	}

	void release(const RecordID id)
	{
		std::lock_guard guard(map_mtx);
		map.erase(map.find(id));
	}
}

RecordFlyweight::RecordFlyweight( RecordID id ) : std::shared_ptr<RecordBase>(internal::getSharedPtr(id)), m_id(id)
{
}

RecordFlyweight::~RecordFlyweight() noexcept
{
	if(this->use_count() == 1) internal::release(m_id);
}