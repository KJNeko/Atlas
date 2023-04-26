//
// Created by kj16609 on 3/28/23.
//

#ifndef ATLAS_IMPORTPREPROCESSOR_HPP
#define ATLAS_IMPORTPREPROCESSOR_HPP

//clang-format: off

#include <QObject>
#include <QString>
#include <QThread>

//WHAT IN GODS NAME
#include <filesystem>

//clang-format: on

struct GameImportData
{
	std::filesystem::path path {};
	QString title { "SOMETHING WENT WRONG" };
	QString creator { "SOMETHING WENT WRONG" };
	QString engine { "SOMETHING WENT WRONG" };
	QString version { "SOMETHING WENT WRONG" };
	std::size_t size {};
	std::vector< std::filesystem::path > executables {};
	std::filesystem::path executable {};
};

class ImportPreProcessor : public QObject
{
	Q_OBJECT

	std::chrono::time_point< std::chrono::steady_clock > last_update { std::chrono::steady_clock::now() };

	std::atomic< bool > abort_task { false };
	std::atomic< bool > running { false };

  public:

	ImportPreProcessor();

	~ImportPreProcessor() = default;

  public slots:
	/**
	 *
	 * @param regex Regex to scan with
	 * @param base Base filepath to scan
	 * @param skip_filesize Causes the filesize calculation to be skipped. Increasing processing speed by a large amount.
	 */
	void processDirectory( const QString regex, const std::filesystem::path base );

	void abort();

  signals:
	void finishedProcessing();
	void finishedDirectory( const GameImportData info );
};

#endif //ATLAS_IMPORTPREPROCESSOR_HPP
