//
// Created by kj16609 on 4/4/23.
//

#ifndef ATLAS_SYSTEM_HPP
#define ATLAS_SYSTEM_HPP

namespace sys
{

#ifdef __linux__
	[[maybe_unused]] constexpr bool is_linux { true };
#else
	[[maybe_unused]] constexpr bool is_linux { false };
#endif

#ifdef _WIN32
	[[maybe_unused]] constexpr bool is_windows { true };
#else
	[[maybe_unused]] constexpr bool is_windows { false };
#endif

} // namespace sys

#endif //ATLAS_SYSTEM_HPP
