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
bool isEngine( const std::filesystem::path& path );

//std::vector<std::filesystem::path> createFileList(const std::filesystem::path& path);

Engine determineEngine( const std::filesystem::path );
QString engineName( const Engine engine );

#endif //HYDRUS95_ENGINEDETECTION_HPP
