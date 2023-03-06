//
// Created by kj16609 on 1/27/23.
//

#ifndef HYDRUS95_LOGGING_HPP
#define HYDRUS95_LOGGING_HPP

#include <filesystem>

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wstrict-overflow"

#include <spdlog/spdlog.h>
#include <fmt/ranges.h>

#pragma GCC diagnostic pop

#include <QString>

void initLogging();

template<> struct fmt::formatter< QString > : fmt::formatter< std::string >
{
	auto format( QString my, format_context& ctx ) -> decltype( ctx.out() )
	{
		return format_to( ctx.out(), "{}", my.toStdString() );
	}
};

#endif	//HYDRUS95_LOGGING_HPP
