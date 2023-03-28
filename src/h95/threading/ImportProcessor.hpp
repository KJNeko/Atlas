//
// Created by kj16609 on 3/28/23.
//

#ifndef HYDRUS95_IMPORTPROCESSOR_HPP
#define HYDRUS95_IMPORTPROCESSOR_HPP

#include <filesystem>

#include <QObject>
#include <QString>
#include <QThread>

struct GameImportData
{
	std::filesystem::path path {};
	QString title {"SOMETHING WENT WRONG"};
	QString version {"SOMETHING WENT WRONG"};
	QString creator {"SOMETHING WENT WRONG"};
	std::size_t size {};
	std::vector< std::filesystem::path > executables {};
	std::filesystem::path executable {};
	bool move_after_import {false};
};

class ImportProcessor : public QObject
{
	Q_OBJECT

	QThread thread {};

  public:

	ImportProcessor();

	~ImportProcessor();

  public slots:
	void processDirectory( const QString regex, const std::filesystem::path base, const bool move_imported, const bool skip_filesize );

  signals:
	void finishedProcessing();
	void finishedDirectory( const GameImportData info );
};

#endif //HYDRUS95_IMPORTPROCESSOR_HPP
