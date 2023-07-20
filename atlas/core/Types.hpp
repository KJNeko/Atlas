//
// Created by kj16609 on 1/15/23.
//

#ifndef ATLAS_TYPES_HPP
#define ATLAS_TYPES_HPP

#include <cstdint>

using RecordID = std::uint32_t;
using AtlasID = std::uint64_t;
using F95ID = std::uint64_t;
[[maybe_unused]] constexpr AtlasID INVALID_ATLAS_ID { 0 };
[[maybe_unused]] constexpr RecordID INVALID_RECORD_ID { 0 };
[[maybe_unused]] constexpr F95ID INVALID_F95_ID { 0 };

#endif //ATLAS_TYPES_HPP
