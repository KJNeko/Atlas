//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_SITERECORD_HPP
#define HYDRUS95_SITERECORD_HPP

#include <chrono>
#include <string>
#include <vector>
#include "backend/tags/Tag.hpp"

class SiteRecord
{
    //! Time of the last recheck or manual update
    std::chrono::time_point<std::chrono::steady_clock> last_updated;

    //! Tags associated with this record
    std::vector<Tag> tags;

    //! Returns a url to the main page of interest for this record.
    std::string getUrl();
};

#endif //HYDRUS95_SITERECORD_HPP
