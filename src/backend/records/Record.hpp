//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_RECORD_HPP
#define HYDRUS95_RECORD_HPP

#include <cstdint>
#include <string>
#include <chrono>
#include <variant>
#include <vector>
#include <memory>

#include "SiteRecord.hpp"

using RecordID = uint32_t;

class Record
{
    //! Interal record id
    RecordID record_id;

    //! All known site records
    std::vector<std::unique_ptr<SiteRecord>> site_records;
};


#endif //HYDRUS95_RECORD_HPP
