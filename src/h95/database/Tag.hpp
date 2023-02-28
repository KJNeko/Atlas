//
// Created by kj16609 on 1/18/23.
//

#ifndef HYDRUS95_TAG_HPP
#define HYDRUS95_TAG_HPP

#include <QString>

using InternalTagID = uint32_t;
using F95TagID = uint16_t;
using DLSiteTagID = uint16_t;


//! Basic tag representation (To fill out more later)
struct Tag
{
	//! Tag id for internal use within h95
	InternalTagID tag_id { 0 };

	//! Text of the tag
	QString text {};

	Tag() = delete;
};

#endif	//HYDRUS95_TAG_HPP
