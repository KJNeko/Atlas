//
// Created by kj16609 on 4/1/23.
//

#ifndef HYDRUS95_ENGINEDETECTION_HPP
#define HYDRUS95_ENGINEDETECTION_HPP

#include <filesystem>
#include <vector>

#include <QString>

enum Engine : int
{
	ENGINES_BEGIN = 0,
	RenPy, // #22
	Unity, // #23
	Unreal,
	RPGM, // #24
	WolfRPG,
	HTML,
	VisualNovelMaker,
	TyanoBuilder,
	Java,
	Flash,
	RAGS,
	KiriKiri,
	NScripter,
	NVList,
	Sukai2,
	ENGINES_END
};

template < Engine engine >
bool isEngine( const std::filesystem::path& path, const std::filesystem::path& executable_path );

template < Engine engine >
QString engineNameT();

//std::vector<std::filesystem::path> createFileList(const std::filesystem::path& path);

//! Returns an engine type of ENGINES_END if no engine is determined
Engine determineEngine( const std::filesystem::path& path, const std::filesystem::path& executable_path );

//! Returns a string name of the engine
QString engineName( const Engine engine );

#endif //HYDRUS95_ENGINEDETECTION_HPP
