//
// Created by kj16609 on 4/1/23.
//

#ifndef ATLAS_ENGINEDETECTION_HPP
#define ATLAS_ENGINEDETECTION_HPP

#include <QString>

#include <filesystem>
#include <vector>

namespace atlas::utils
{
	class FileScanner;
}

enum Engine : int
{
	ENGINES_BEGIN = 0,
	RenPy, // #22
	Unity, // #23
	Unreal,
	RPGM, // #24
	WolfRPG,
	VisualNovelMaker,
	TyanoBuilder,
	Java,
	Flash,
	RAGS,
	KiriKiri,
	NScripter,
	NVList,
	Sukai2,
	HTML,
	QSP,
	BAT,
	MonoGame,
	GamesforLive,
	XNA,
	Adobe_AIR,

	ENGINES_END, // This stays at the bottom
	UNKNOWN, // This stays under ENGINES_END
};

//! Function to be specialized for each Engine to return true if the engine is valid.
template < Engine engine >
bool isEngineT( atlas::utils::FileScanner& scanner );

bool checkEngineType( std::string engine, atlas::utils::FileScanner& scanner );
//! String name of the engine.
template < Engine engine >
QString engineNameT();

//std::vector<std::filesystem::path> createFileList(const std::filesystem::path& path);

//! Returns an engine type of ENGINES_END if no engine is determined
Engine determineEngine( atlas::utils::FileScanner& scanner );

//! Returns a string name of the engine
QString engineName( const Engine engine );

std::vector< std::filesystem::path > detectExecutables( atlas::utils::FileScanner& scanner );

std::vector< std::filesystem::path >
	scoreExecutables( std::vector< std::filesystem::path > paths, const Engine engine = UNKNOWN );

#endif //ATLAS_ENGINEDETECTION_HPP
