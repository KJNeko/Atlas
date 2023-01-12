//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_F95RECORD_HPP
#define HYDRUS95_F95RECORD_HPP

#include <cstdint>
#include "SiteRecord.hpp"

class F95Record : public SiteRecord
{
    using F95ID = uint32_t;

    F95ID f95_id;
};

#endif //HYDRUS95_F95RECORD_HPP
