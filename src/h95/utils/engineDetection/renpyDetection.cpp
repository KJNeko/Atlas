//
// Created by kj16609 on 4/1/23.
//

#include "engineDetection.hpp"

template <>
bool isEngine< RenPy >( const std::filesystem::path& path )
{
	return std::filesystem::exists( path / "renpy" );
}
