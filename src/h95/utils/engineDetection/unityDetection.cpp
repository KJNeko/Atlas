//
// Created by kj16609 on 4/1/23.
//

#include "engineDetection.hpp"

template <>
bool isEngine< Unity >( const std::filesystem::path& path )
{
	return std::filesystem::exists( path / "Data" / "Managed" / "Assembly-CSharp.dll" );
}

template <>
QString engineNameT< Unity >()
{
	return "Unity";
}