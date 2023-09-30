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

#ifndef ATLAS_GIT_CREATED_TIME
#error "Unable to get time. Specify the tag with -DATLAS_GIT_TIME"
#endif

#else

#define ATLAS_VERSION "Debug Build. DO NOT REDISTRIBUTE"
#define ATLAS_GIT_BRANCH "NOT SET DURING BUILD PROCESS"
#define ATLAS_GIT_REVISION "NOT SET DURING BUILD PROCESS"
#define ATLAS_GIT_TAG "NOT SET DURING BUILD PROCESS"
#define ATLAS_GIT_CREATED_TIME "NOT SET DURING BUILD PROCESS"

#endif

#ifndef NDEBUG

//Tag-Brief | Branch
#define ATLAS_VERSION_STRING ATLAS_GIT_TAG "-" ATLAS_GIT_REVISION_BRIEF " | " ATLAS_GIT_BRANCH

#else

#define ATLAS_VERSION_STRING ATLAS_GIT_TAG

#endif

//#define ATLAS_VERSION_STR TOSTRING( ATLASVERSION )

namespace utils
{
	[[maybe_unused]] constexpr std::string_view version_string { ATLAS_VERSION_STRING };
	[[maybe_unused]] constexpr std::string_view git_branch { ATLAS_GIT_BRANCH };
	[[maybe_unused]] constexpr std::string_view git_revision { ATLAS_GIT_REVISION };
	[[maybe_unused]] constexpr std::string_view git_rev_brief { ATLAS_GIT_REVISION_BRIEF };
	[[maybe_unused]] constexpr std::string_view git_tag { ATLAS_GIT_TAG };
	[[maybe_unused]] constexpr std::string_view git_time { ATLAS_GIT_CREATED_TIME };

	inline const QString version_string_qt()
	{
		const QString version {
			QString::fromLocal8Bit( git_tag.data(), static_cast< qsizetype >( git_tag.size() ) )
		};

		const QString branch {
			QString::fromLocal8Bit( git_branch.data(), static_cast< qsizetype >( git_branch.size() ) )
		};

		std::tm t = {};
		std::istringstream ss( QString::fromLocal8Bit( git_time.data(), static_cast< qsizetype >( git_time.size() )).toStdString());
		if (ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
		{
			const long int unix_time {static_cast<long int>(std::mktime( &t ))};
			return version + "-" + QString::number( unix_time ).left( QString::number( unix_time ).length() - 3 ) + " | " + branch ;
		}
		else{
			return version + "- UNKNOWN | " + branch;
		}
	}
} // namespace utils

#endif //ATLASGAMEMANAGER_VERSION_HPP
