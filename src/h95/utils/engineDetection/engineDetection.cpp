//
// Created by kj16609 on 4/1/23.
//

#include "engineDetection.hpp"

template <>
bool isEngine< ENGINES_BEGIN >( [[maybe_unused]] const std::filesystem::path& path )
{
	return false;
}

template <>
bool isEngine< ENGINES_END >( [[maybe_unused]] const std::filesystem::path& path )
{
	return true;
}

template < Engine engine >
Engine findEngine( const std::filesystem::path& path )
{
	if constexpr ( engine == ENGINES_END )
		return ENGINES_END;
	else
	{
		if ( isEngine< engine >( path ) )
			return engine;
		else
			return findEngine< static_cast< Engine >( engine + 1 ) >( path );
	}
}

Engine determineEngine( const std::filesystem::path path )
{
	return findEngine< ENGINES_BEGIN >( path );
}

QString engineName( const Engine engine )
{
	switch ( engine )
	{
		case RenPy:
			return "Ren'Py";
		case Unity:
			return "Unity";
		case Unreal:
			return "Unreal Engine";
		case RPGM:
			return "RPG Maker";
		case WolfRPG:
			return "Wolf RPG Editor";
		case HTML:
			return "HTML";
		case VisualNovelMaker:
			return "Visual Novel Maker";
		case TyanoBuilder:
			return "Tyano Builder";
		case Java:
			return "Java";
		case Flash:
			return "Flash";
		case RAGS:
			return "RAGS";
		case KiriKiri:
			return "KiriKiri";
		case NScripter:
			return "NScripter";
		case NVList:
			return "NVList";
		case Sukai2:
			return "Sukai2";
		case ENGINES_END:
			[[fallthrough]];
		case ENGINES_BEGIN:
			[[fallthrough]];
		defauolt:
			return "Unknown";
	}
}
