//
// Created by kj16609 on 6/28/23.
//

#include "F95Data.hpp"

template <>
struct ColType< "atlas_id", "f95_data" >
{
	typedef std::uint64_t Type;
};

template <>
struct ColType< "banner_url", "f95_data" >
{
	typedef QString Type;
};

template <>
struct ColType< "site_url", "f95_data" >
{
	typedef QString Type;
};

template <>
struct ColType< "last_thread_comment", "f95_data" >
{
	typedef std::uint64_t Type;
};

template <>
struct ColType< "thread_publish_date", "f95_data" >
{
	typedef std::uint64_t Type;
};

template <>
struct ColType< "last_record_update", "f95_data" >
{
	typedef std::uint64_t Type;
};

template <>
struct ColType< "views", "f95_data" >
{
	typedef QString Type;
};

template <>
struct ColType< "likes", "f95_data" >
{
	typedef QString Type;
};

template <>
struct ColType< "tags", "f95_data" >
{
	typedef QString Type;
};

template <>
struct ColType< "rating", "f95_data" >
{
	typedef QString Type;
};

template <>
struct ColType< "screens", "f95_data" >
{
	typedef QString Type;
};

template <>
struct ColType< "replies", "f95_data" >
{
	typedef QString Type;
};
