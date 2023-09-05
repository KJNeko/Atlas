//
// Created by kj16609 on 5/9/23.
//

#ifndef ATLASGAMEMANAGER_VERSION_HPP
#define ATLASGAMEMANAGER_VERSION_HPP

#ifdef NDEBUG
#ifndef ATLASVERSION
#error "No version specified. Define with -DATLASVERSION during config (-DATLASVERSION=\"test\") "
#endif
#else
#ifndef ATLASVERSION
#define ATLASVERSION "Debug Build. DO NOT REDISTRIBUTE"
#endif
#endif

#define VAL( str ) #str
#define TOSTRING( str ) VAL( str )

#ifdef ATLASVERSION
#define ATLASVERSION_STR TOSTRING( ATLASVERSION )
#endif

namespace utils
{
	constexpr std::string_view version_string { ATLASVERSION_STR };

	inline const QString version_string_qt()
	{
		return QString::fromLocal8Bit( version_string.data(), static_cast< qsizetype >( version_string.size() ) );
	}
} // namespace utils

#endif //ATLASGAMEMANAGER_VERSION_HPP
