//
// Created by kj16609 on 6/30/23.
//

#ifndef ATLASGAMEMANAGER_ATLASCOLTYPE_HPP
#define ATLASGAMEMANAGER_ATLASCOLTYPE_HPP

#include "core/database/Column.hpp"

enum class AtlasColumns
{
	IdName,
	ShortName,
	Title,
	OriginalName,
	Category,
	Engine,
	Status,
	Version,
	Developer,
	Creator,
	Overview,
	Censored,
	Language,
	Translations,
	Genre,
	Tags,
	Voice,
	OS,
	ReleaseDate,
	Length,
	Banner,
	BannerWide,
	Cover,
	Logo,
	Wallpaper,
	Previews,
	LastDbUpdate
};

template < AtlasColumns col >
struct AtlasColInfo;

template < AtlasColumns col >
using AtlasColType = AtlasColInfo< col >::Type;

template <>
struct AtlasColInfo< AtlasColumns::IdName >
{
	using Type = QString;
	static constexpr fgl::string_literal col_name { "id_name" };
};

template <>
struct AtlasColInfo< AtlasColumns::Title >
{
	using Type = QString;
	static constexpr fgl::string_literal col_name { "title" };
};

template <>
struct AtlasColInfo< AtlasColumns::Creator >
{
	using Type = QString;
	static constexpr fgl::string_literal col_name { "creator" };
};

template <>
struct AtlasColInfo< AtlasColumns::Engine >
{
	using Type = QString;
	static constexpr fgl::string_literal col_name { "engine" };
};

#endif //ATLASGAMEMANAGER_ATLASCOLTYPE_HPP
