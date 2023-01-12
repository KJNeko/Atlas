//
// Created by kj16609 on 1/12/23.
//

#ifndef HYDRUS95_TAG_HPP
#define HYDRUS95_TAG_HPP

#include <string>
#include <variant>


namespace internal {
    template<typename T>
    class BasicWrapper {
        T value;
    public:
        BasicWrapper(T &&value_in) : value(std::forward<T>(value)) {}

        operator T &() { return value; }
    };
}

using DLSiteTagID = std::string;
using F95TagID = internal::BasicWrapper<uint32_t>;

class Tag
{
    using TagID = uint32_t;

    //! Text of the record
    std::string text;

    //! Tag ID of the record (For it's respective site)
    std::variant<DLSiteTagID, F95TagID> tag_id;
};

#endif //HYDRUS95_TAG_HPP
