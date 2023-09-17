//
// Created by kj16609 on 5/9/23.
//

#ifndef ATLASGAMEMANAGER_VERSION_HPP
#define ATLASGAMEMANAGER_VERSION_HPP

#ifndef BYPASS_ATLAS_VERSION_STRINGS_REQUIRED

#ifndef ATLAS_GIT_REVISION
#error "No git commit specified. Specify the revision -DATLAS_GIT_REVISION"
#endif

#ifndef ATLAS_GIT_REVISION_BRIEF
#error "No git commit brief specified. Specify the revision -DATLAS_GIT_REVISION_BRIEF"
#endif

#ifndef ATLAS_GIT_BRANCH
#error "No git branch specified. Specify the branch via -DATLAS_GIT_BRANCH"
#endif

#ifndef ATLAS_GIT_TAG
#error "No git tag specified. Specify the tag with -DATLAS_GIT_TAG"
#endif

#else

#define ATLAS_VERSION "Debug Build. DO NOT REDISTRIBUTE"
#define ATLAS_GIT_BRANCH "NOT SET DURING BUILD PROCESS"
#define ATLAS_GIT_REVISION "NOT SET DURING BUILD PROCESS"
#define ATLAS_GIT_TAG "NOT SET DURING BUILD PROCESS"

#endif

#define VAL( str ) #str
#define TOSTRING( str ) VAL( str )

#ifndef NDEBUG

//Tag-Brief | Branch
#define ATLAS_VERSION_STRING                                                                                           \
	TOSTRING( ATLAS_GIT_TAG ) "-" TOSTRING( ATLAS_GIT_REVISION_BRIEF ) " | " TOSTRING( ATLAS_GIT_BRANCH )

#else

#define ATLAS_VERSION_STRING TOSTRING( ATLAS_GIT_TAG )

#endif

#define ATLAS_GIT_BRANCH_STRING TOSTRING( ATLAS_GIT_BRANCH )
#define ATLAS_GIT_REVISION_STRING TOSTRING( ATLAS_GIT_REVISION )
#define ATLAS_GIT_REVISION_BRIEF_STRING TOSTRING( ATLAS_GIT_REVISION_BRIEF )
#define ATLAS_GIT_TAG_STRING TOSTRING( ATLAS_GIT_TAG )

//#define ATLAS_VERSION_STR TOSTRING( ATLASVERSION )

namespace utils
{
	[[maybe_unused]] constexpr std::string_view version_string { ATLAS_VERSION_STRING };
	[[maybe_unused]] constexpr std::string_view git_branch { ATLAS_GIT_BRANCH_STRING };
	[[maybe_unused]] constexpr std::string_view git_revision { ATLAS_GIT_REVISION_STRING };
	[[maybe_unused]] constexpr std::string_view git_rev_brief { ATLAS_GIT_REVISION_BRIEF_STRING };
	[[maybe_unused]] constexpr std::string_view git_tag { ATLAS_GIT_TAG_STRING };

	inline const QString version_string_qt()
	{
		return QString::fromLocal8Bit( version_string.data(), static_cast< qsizetype >( version_string.size() ) );
	}
} // namespace utils

#endif //ATLASGAMEMANAGER_VERSION_HPP
