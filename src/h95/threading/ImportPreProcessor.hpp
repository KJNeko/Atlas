//
// Created by kj16609 on 3/28/23.
//

#ifndef HYDRUS95_IMPORTPREPROCESSOR_HPP
#define HYDRUS95_IMPORTPREPROCESSOR_HPP

#include <filesystem>

#include <QObject>
#include <QString>
#include <QThread>

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

	std::chrono::time_point<std::chrono::steady_clock> last_update {std::chrono::steady_clock::now()};
	std::vector<GameImportData> buffer {};

	std::atomic<bool> abort_task { false };

  public:

	ImportPreProcessor();

	~ImportPreProcessor() = default;

  public slots:
	/**
	 *
	 * @param regex Regex to scan with
	 * @param base Base filepath to scan
	 * @param move_imported Default move state for all records when processed. (Can be set individually after)
	 * @param skip_filesize Causes the filesize calculation to be skipped. Increasing processing speed by a large amount.
	 */
	void processDirectory(
		const QString regex, const std::filesystem::path base, const bool move_imported, const bool skip_filesize );

	void abort();

  signals:
	void finishedProcessing();
	void finishedDirectory( const std::vector<GameImportData> info );
};

#endif //HYDRUS95_IMPORTPREPROCESSOR_HPP
