//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_DLSITERECORD_HPP
#define HYDRUS95_DLSITERECORD_HPP

#include <string>
#include "SiteRecord.hpp"

class DLSiteRecord : public SiteRecord
{
    using DLSiteID = std::string;

    DLSiteID dlsite_id;
};

#endif //HYDRUS95_DLSITERECORD_HPP
