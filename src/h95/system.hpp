//
// Created by kj16609 on 4/4/23.
//

#ifndef HYDRUS95_SYSTEM_HPP
#define HYDRUS95_SYSTEM_HPP

namespace sys
{

#ifdef __unix__
	[[maybe_unused]] constexpr bool is_unix { true };
#else
	[[maybe_unused]] constexpr bool is_unix { false };
#endif

#ifdef _WIN32
	[[maybe_unused]] constexpr bool is_windows { true };
#else
	[[maybe_unused]] constexpr bool is_windows { false };
#endif

} // namespace sys

#endif //HYDRUS95_SYSTEM_HPP
